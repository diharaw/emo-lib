#include <iostream>
#include <string>
#include <dirent.h>
#include <sndfile.hh>
#include <xtract/libxtract.h>
#include <xtract/xtract_stateful.h>
#include <xtract/xtract_scalar.h>
#include <xtract/xtract_helper.h>

static const char* g_emotions_a[] =
{
    "neutral",
    "anger",
    "fear",
    "happy",
    "sadness",
    "surprise"
};

static const char* g_emotions_b[] =
{
    "neutral",
    "anger",
    "fear",
    "happy",
    "sadness",
    "disgust"
};

static const char** g_label_types[] =
{
    g_emotions_a,
    g_emotions_b
};

static const char* g_subtypes[] =
{
    "SF_FORMAT_PCM_S8",
    "SF_FORMAT_PCM_16",
    "SF_FORMAT_PCM_24",
    "SF_FORMAT_PCM_32",
    "SF_FORMAT_PCM_U8",
    "SF_FORMAT_FLOAT",
    "SF_FORMAT_DOUBLE"
};

#define BLOCK_SIZE 512
#define HALF_BLOCK_SIZE BLOCK_SIZE >> 1
#define MAVG_COUNT 10
#define MFCC_FREQ_BANDS 13
#define MFCC_FREQ_MIN 20
#define MFCC_FREQ_MAX 20000
#define SAMPLERATE 48000

int main(int argc, const char * argv[])
{
    double *window = NULL;
    double *window_subframe = NULL;
    double mfccs[MFCC_FREQ_BANDS] = {0};
    double windowed[BLOCK_SIZE] = {0};
    double spectrum[BLOCK_SIZE] = {0};
    double argd[4] = {0};
    
    // Path to dataset root.
    if(argc != 3)
    {
        std::cout << "usage: convert_imageset <path_to_dataset_root> <label_type>" << std::endl;
        return -1;
    }
    
    std::string dataset_root = std::string(argv[1]);
    int label_type = atoi(argv[2]);
    
    std::string full_path = dataset_root + "/2 - PARTITIONED/train/";
    
    xtract_mel_filter mel_filters;
    xtract_last_n_state *last_n_state = xtract_last_n_state_new(MAVG_COUNT);
    
    /* Allocate Mel filters */
    mel_filters.n_filters = MFCC_FREQ_BANDS;
    mel_filters.filters   = (double **)malloc(MFCC_FREQ_BANDS * sizeof(double *));
    for(uint8_t k = 0; k < MFCC_FREQ_BANDS; ++k)
    {
        mel_filters.filters[k] = (double *)malloc(BLOCK_SIZE * sizeof(double));
    }
    
    for(uint32_t i = 0; i < 6; i++)
    {
        std::string path_with_emotion = full_path + std::string(g_label_types[label_type][i]);
        
        std::cout << std::endl;
        std::cout << "Processing Emotion : " << g_label_types[label_type][i] << std::endl;
        std::cout << std::endl;
        
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir (path_with_emotion.c_str())) != NULL)
        {
            /* print all the files and directories within directory */
            while ((ent = readdir (dir)) != NULL)
            {
                std::string fileName = std::string(ent->d_name);
                
                if(fileName.length() > 2)
                {
                    std::string path = path_with_emotion + "/";
                    path += fileName;
                    
                    SndfileHandle file = SndfileHandle(path);
                    
                    std::cout << std::endl;
                    printf ("    File        : %s\n", fileName.c_str()) ;
                    printf ("    Sample rate : %d\n", file.samplerate ()) ;
                    printf ("    Channels    : %d\n", file.channels ()) ;
                    printf ("    Frames      : %lld\n", file.frames ()) ;
                    printf ("    Format      : %d\n", (file.format () & SF_FORMAT_TYPEMASK)) ;
                    printf ("    Subtype     : %s\n", g_subtypes[(file.format () & SF_FORMAT_SUBMASK) - 1]) ;
                    
                    file.seek(0, SF_SEEK_SET);
                    
                    short* buffer = (short*)malloc(file.frames() * sizeof(short));
                    file.read(buffer, file.frames());
                    
                    double* buffer_d = (double*)malloc(file.frames() * sizeof(double));
                    
                    for(uint64_t i = 0; i < file.frames(); i++)
                        buffer_d[i] = buffer[i];
                    
                    int frame_count = 0;
                    
                    xtract_init_mfcc(BLOCK_SIZE >> 1, file.samplerate() >> 1, XTRACT_EQUAL_GAIN, MFCC_FREQ_MIN, MFCC_FREQ_MAX, mel_filters.n_filters, mel_filters.filters);
                    
                    /* create the window functions */
                    window = xtract_init_window(BLOCK_SIZE, XTRACT_HANN);
                    window_subframe = xtract_init_window(HALF_BLOCK_SIZE, XTRACT_HANN);
                    xtract_init_wavelet_f0_state();
                    
                    for(uint64_t i = 0; (i + BLOCK_SIZE) < file.frames(); i += HALF_BLOCK_SIZE) // half overlap
                    {
                        frame_count++;
                        xtract_windowed(&buffer_d[i], BLOCK_SIZE, window, windowed);
                        
                        /* get the spectrum */
                        argd[0] = SAMPLERATE / (double)BLOCK_SIZE;
                        argd[1] = XTRACT_MAGNITUDE_SPECTRUM;
                        argd[2] = 0.f; /* DC component - we expect this to zero for square wave */
                        argd[3] = 0.f; /* No Normalisation */
                        
                        xtract_init_fft(BLOCK_SIZE, XTRACT_SPECTRUM);
                        xtract[XTRACT_SPECTRUM](windowed, BLOCK_SIZE, &argd[0], spectrum);
                        xtract_free_fft();
                        
                        /* compute the MFCCs */
                        xtract_mfcc(spectrum, BLOCK_SIZE >> 1, &mel_filters, mfccs);
                        
//                        std::cout << "Frame : " << i << std::endl;
//                        for(uint32_t i = 0; i < MFCC_FREQ_BANDS; i++)
//                        {
//                            printf ("    MFCC %d        : %f\n", i + 1, mfccs[i]) ;
//                        }
//                        std::cout << std::endl;
                    }
                    
                    std::cout << "Window Count : " << frame_count << std::endl;
                    
                    xtract_free_window(window);
                    xtract_free_window(window_subframe);
                    
                    free(buffer);
                    free(buffer_d);
//                    return 0;
                }
            }
            
            closedir (dir);
        }
        else
        {
            /* could not open directory */
            perror ("");
            return EXIT_FAILURE;
        }
    }
    
    /* cleanup */
    for(int n = 0; n < MFCC_FREQ_BANDS; ++n)
    {
        free(mel_filters.filters[n]);
    }
    free(mel_filters.filters);
    
    return 0;
}
