//
//  main.cpp
//  audio_feature_extractor
//
//  Created by Dihara Wijetunga on 9/16/17.
//  Copyright © 2017 fidenz. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <dirent.h>
#include "mfcc.cpp"

#define DELTA_N 2

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

char* getCmdOption(char **begin, char **end, const std::string &value)
{
    char **iter = std::find(begin, end, value);
    if (iter != end && ++iter != end)
        return *iter;
    return nullptr;
}

void compute_delta(std::vector<v_d>& input, uint32_t n, std::vector<v_d>& output)
{
    uint32_t count = input.size();
    
    for(uint32_t i = 0; i < count; i++)
    {
        uint32_t bands = input[i].size();
        
        for(uint32_t j = 0; j < bands; j++)
        {
            double numerator = 0;
            double denominator = 0;
            
            for(uint32_t k = 1; k <= n; k++)
            {
                int32_t i1 = i + k;
                int32_t i2 = i - k;
                
                if(i1 > (count - 1))
                    i1 = count - 1;
                
                if(i2 < 0)
                    i2 = 0;
                
                numerator += k * (input[i1][j] - input[i2][j]);
                denominator += pow(k, 2);
            }
            
            output[i].push_back(numerator / (2.0 * denominator));
        }
    }
}

void print_vector(std::vector<v_d>& vec, std::string name)
{
    int j = 0;
    
    for(auto& frame : vec)
    {
        std::cout << "Frame " << j << std::endl;
        int i = 0;
        
        for(auto& coef : frame)
        {
            std::cout << name << " " << i << " : " << coef << std::endl;
            i++;
        }
        
        j++;
        
        std::cout << std::endl;
    }
}

int process_dataset (MFCC &mfccComputer, const char* dataset, int labeltype, int verbose)
{
    std::string train_path = std::string(dataset) + "/2 - PARTITIONED/train/";
    
    for(uint32_t i = 0; i < 6; i++)
    {
        std::string path_with_emotion = train_path + std::string(g_label_types[labeltype][i]);
        
        std::cout << std::endl;
        std::cout << "Processing Emotion : " << g_label_types[labeltype][i] << std::endl;
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
                    // Initialise input and output streams
                    std::ifstream wavFp;
                    
                    std::string path = path_with_emotion + "/";
                    path += fileName;
                    
                    // Check if input is readable
                    wavFp.open(path);
                    if (!wavFp.is_open())
                    {
                        std::cerr << "Unable to open input file: " << path << std::endl;
                        return 1;
                    }
                    
                    // Extract and write features
                    std::vector<v_d> mfccs = mfccComputer.process_buffer(wavFp);
                    std::vector<v_d> deltas;
                    std::vector<v_d> delta_deltas;
                    
                    deltas.resize(mfccs.size());
                    delta_deltas.resize(mfccs.size());
                    
                    // Extract delta of MFCC
                    compute_delta(mfccs, DELTA_N, deltas);
                    
                    // Extract delta-delta of MFCC
                    compute_delta(deltas, DELTA_N, delta_deltas);
                    
                    if(mfccs.size() == 0)
                    {
                        std::cerr << "Error processing " << path << std::endl;
                        return -1;
                    }
                    
                    std::cout << "Processed File : " << path << std::endl;
                    
                    if(verbose)
                    {
                        print_vector(mfccs, "MFCC");
                        print_vector(deltas, "MFCC Delta");
                        print_vector(delta_deltas, "MFCC Delta-Delta");
                    }
                    
                    wavFp.close();
                }
            }
        }
    }
    
    return 0;
}

int main(int argc, char * argv[])
{
    std::string USAGE = "convert_audioset : lmdb audio dataset converter\n";
    USAGE += "OPTIONS\n";
    USAGE += "--numcepstra      : Number of output cepstra, excluding log-energy (default=12)\n";
    USAGE += "--numfilters      : Number of Mel warped filters in filterbank (default=40)\n";
    USAGE += "--samplingrate    : Sampling rate in Hertz (default=16000)\n";
    USAGE += "--winlength       : Length of analysis window in milliseconds (default=25)\n";
    USAGE += "--frameshift      : Frame shift in milliseconds (default=10)\n";
    USAGE += "--lowfreq         : Filterbank low frequency cutoff in Hertz (default=20)\n";
    USAGE += "--highfreq        : Filterbank high freqency cutoff in Hertz (default=samplingrate/2)\n";
    USAGE += "--dataset         : Path to the root folder of the dataset\n";
    USAGE += "--labeltype       : The label type to use (default=0)\n";
    USAGE += "--verbose         : Print MFCC output (default=0)\n";
    USAGE += "USAGE EXAMPLES\n";
    USAGE += "./convert_audioset --dataset RAVDESS --labeltype 0 --numcepstra 12 --samplingrate 48000\n";
    USAGE += "./convert_audioset --dataset EMODB --labeltype 1 --numcepstra 12 --samplingrate 16000\n";
    
    char *num_cepstra_arg = getCmdOption(argv, argv+argc, "--numcepstra");
    char *num_filters_arg = getCmdOption(argv, argv+argc, "--numfilters");
    char *sampling_rate_arg = getCmdOption(argv, argv+argc, "--samplingrate");
    char *win_length_arg = getCmdOption(argv, argv+argc, "--winlength");
    char *frame_shift_arg = getCmdOption(argv, argv+argc, "--frameshift");
    char *low_freq_arg = getCmdOption(argv, argv+argc, "--lowfreq");
    char *high_freq_arg = getCmdOption(argv, argv+argc, "--highfreq");
    char *dataset_arg = getCmdOption(argv, argv+argc, "--dataset");
    char *labeltype_arg = getCmdOption(argv, argv+argc, "--labeltype");
    char *verbose_arg = getCmdOption(argv, argv+argc, "--verbose");
    
    // Check arguments
    if(argc < 3 || !dataset_arg)
    {
        std::cout << "ERROR: Incorrect arguments.\n";
        std::cout << USAGE;
        return 1;
    }
    
    // Assign variables
    int num_cepstra = (num_cepstra_arg ? atoi(num_cepstra_arg) : 12);
    int num_filters = (num_filters_arg ? atoi(num_filters_arg) : 40);
    int sampling_rate = (sampling_rate_arg ? atoi(sampling_rate_arg) : 16000);
    int win_length = (win_length_arg ? atoi(win_length_arg) : 20);
    int frame_shift = (frame_shift_arg ? atoi(frame_shift_arg) : 10);
    int low_freq = (low_freq_arg ? atoi(low_freq_arg) : 20);
    int high_freq = (high_freq_arg ? atoi(high_freq_arg) : sampling_rate/2);
    int labeltype = (labeltype_arg ? atoi(labeltype_arg) : 0);
    int verbose = (verbose_arg ? atoi(verbose_arg) : 0);
    
    // Initialise MFCC class instance
    MFCC mfcc_computer (sampling_rate, num_cepstra, win_length, frame_shift, num_filters, low_freq, high_freq);
    
    return process_dataset(mfcc_computer, dataset_arg, labeltype, verbose);
}
