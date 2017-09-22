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

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <google/protobuf/text_format.h>

#if defined(USE_LEVELDB) && defined(USE_LMDB)
#include <leveldb/db.h>
#include <leveldb/write_batch.h>
#include <lmdb.h>
#endif

#include "boost/scoped_ptr.hpp"
#include <definitions.hpp>
#include <util/db.hpp>
#include <util/format.hpp>
#include <util/rng.hpp>

#include "caffe/proto/caffe.pb.h"

#define DELTA_N 2
#define DB_BACKEND "lmdb"
#define IMAGE_WIDTH 36
#define IMAGE_HEIGHT 36
#define IMAGE_CHANNELS 1

using namespace caffe;
using boost::scoped_ptr;
using std::string;

double g_max_values[IMAGE_HEIGHT];
double g_min_values[IMAGE_HEIGHT];

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

double find_max(std::vector<v_d>& vec, int i)
{
    double max = vec[0][i];
    
    for(auto& frame : vec)
    {
        if(frame[i] > max)
            max = frame[i];
    }
    
    return max;
}

double find_min(std::vector<v_d>& vec, int i)
{
    double min = vec[0][i];
    
    for(auto& frame : vec)
    {
        if(frame[i] < min)
            min = frame[i];
    }
    
    return min;
}

void rescale(std::vector<v_d>& vec)
{
    if(vec.size() > 0)
    {
        int num = vec[0].size();
        
        for(uint32_t i = 0; i < num; i++)
        {
            double max = g_max_values[i];
            double min = g_min_values[i]; // Offset
            double scale = max - min;
            
            for(auto& frame : vec)
            {
                frame[i] = (frame[i] - min)/scale;
            }
        }
    }
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
            
            for(int32_t k = 1; k <= n; k++)
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
        
        
        for(int i = 1; i < frame.size(); i++)
        {
            std::cout << name << " " << i << " : " << frame[i] << std::endl;
        }
        
        j++;
        
        std::cout << std::endl;
    }
}

void find_min_max_phase(MFCC &mfccComputer, const char* dataset, const char* phase, int labeltype, int num_coefs)
{
    std::string train_path = std::string(dataset) + "/3 - TRIMMED/";
    train_path += phase;
    train_path += "/";
    
    bool first = true;
    
    for(uint32_t i = 0; i < 6; i++)
    {
        std::string path_with_emotion = train_path + std::string(g_label_types[labeltype][i]);
        
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
                    
                    // Initialise input and output streams
                    std::ifstream wavFp;
                    
                    // Check if input is readable
                    wavFp.open(path);
                    if (!wavFp.is_open())
                    {
                        std::cerr << "Unable to open input file: " << path << std::endl;
                        return;
                    }
                    
                    // Extract and write features
                    std::vector<v_d> mfccs = mfccComputer.process_buffer(wavFp);
                    
                    if(mfccs.size() == 0)
                    {
                        std::cerr << "Error processing " << path << std::endl;
                        return;
                    }
                    
                    std::vector<v_d> deltas;
                    std::vector<v_d> delta_deltas;
                    
                    deltas.resize(mfccs.size());
                    delta_deltas.resize(mfccs.size());
                    
                    // Extract delta of MFCC
                    compute_delta(mfccs, DELTA_N, deltas);
                    
                    // Extract delta-delta of MFCC
                    compute_delta(deltas, DELTA_N, delta_deltas);
                    
                    // Set initial values
                    if(first)
                    {
                        first = false;
                        
                        for(uint32_t j = 0; j < num_coefs; j++)
                        {
                            g_max_values[j] = mfccs[0][j];
                            g_min_values[j] = mfccs[0][j];
                        }
                        
                        for(uint32_t j = 0; j < num_coefs; j++)
                        {
                            g_max_values[num_coefs + j] = deltas[0][j];
                            g_min_values[num_coefs + j] = deltas[0][j];
                        }
                        
                        for(uint32_t j = 0; j < num_coefs; j++)
                        {
                            g_max_values[num_coefs * 2 + j] = delta_deltas[0][j];
                            g_min_values[num_coefs * 2 + j] = delta_deltas[0][j];
                        }
                    }
                    
                    // MFCC
                    for(uint32_t f = 0; f < num_coefs; f++)
                    {
                        for(uint32_t j = 0; j < mfccs.size(); j++)
                        {
                            if(g_max_values[f] < mfccs[j][f])
                                g_max_values[f] = mfccs[j][f];
                            
                            if(g_min_values[f] > mfccs[j][f])
                                g_min_values[f] = mfccs[j][f];
                        }
                    }
                    
                    // MFCC Delta
                    for(uint32_t f = 0; f < num_coefs; f++)
                    {
                        for(uint32_t j = 0; j < deltas.size(); j++)
                        {
                            if(g_max_values[num_coefs + f] < mfccs[j][f])
                                g_max_values[num_coefs + f] = mfccs[j][f];
                            
                            if(g_min_values[num_coefs + f] > mfccs[j][f])
                                g_min_values[num_coefs + f] = mfccs[j][f];
                        }
                    }
                    
                    // MFCC Delta-delta
                    for(uint32_t f = 0; f < num_coefs; f++)
                    {
                        for(uint32_t j = 0; j < delta_deltas.size(); j++)
                        {
                            if(g_max_values[num_coefs * 2 + f] < mfccs[j][f])
                                g_max_values[num_coefs * 2 + f] = mfccs[j][f];
                            
                            if(g_min_values[num_coefs * 2 + f] > mfccs[j][f])
                                g_min_values[num_coefs * 2 + f] = mfccs[j][f];
                        }
                    }
                }
            }
        }
    }

}

void find_min_max(MFCC &mfccComputer, const char* dataset, int labeltype, int num_coefs)
{
    find_min_max_phase(mfccComputer, dataset, "train", labeltype, num_coefs);
    find_min_max_phase(mfccComputer, dataset, "test", labeltype, num_coefs);
    find_min_max_phase(mfccComputer, dataset, "validate", labeltype, num_coefs);
    
    for(uint32_t i = 0; i < IMAGE_HEIGHT; i++)
        std::cout << "Max Values : " << i << " : " << g_max_values[i] << std::endl;
    
    for(uint32_t i = 0; i < IMAGE_HEIGHT; i++)
        std::cout << "Min Values : " << i << " : " << g_min_values[i] << std::endl;
}

int process_dataset (MFCC &mfccComputer,
                     const char* dataset,
                     const char* phase,
                     const char* dbpath,
                     int labeltype,
                     int verbose,
                     int num_coefs,
                     int rescale_val)
{
    std::string train_path = std::string(dataset) + "/3 - TRIMMED/";
    train_path += phase;
    train_path += "/";
    std::vector<std::pair<std::string, int> > lines;
    
    for(uint32_t i = 0; i < 6; i++)
    {
        std::string path_with_emotion = train_path + std::string(g_label_types[labeltype][i]);
        
        std::cout << std::endl;
        std::cout << "Processing Emotion : " << g_label_types[labeltype][i] << std::endl;
        
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
                    
                    // Insert into line vector with label
                    std::pair<string, int> item;
                    
                    item.first = path;
                    item.second = i;
                    
                    lines.push_back(item);
                }
            }
        }
    }
    
    int32_t item_id = 0;
    
    scoped_ptr<db::DB> db(db::GetDB(DB_BACKEND));
    db->Open(dbpath, db::NEW);
    scoped_ptr<db::Transaction> txn(db->NewTransaction());
    
    Datum datum;
    datum.set_channels(IMAGE_CHANNELS);
    datum.set_height(IMAGE_HEIGHT);
    datum.set_width(IMAGE_WIDTH);
    string value;
    
    // Shuffle data
    shuffle(lines.begin(), lines.end());
    
    std::cout << "File count : " << lines.size() << std::endl;
    
    uint32_t subitems = 0;
    
    // Extract features and write to lmdb
    for(auto& line : lines)
    {
        // Initialise input and output streams
        std::ifstream wavFp;
        
        // Check if input is readable
        wavFp.open(line.first);
        if (!wavFp.is_open())
        {
            std::cerr << "Unable to open input file: " << line.first << std::endl;
            return 1;
        }
        
        // Extract and write features
        std::vector<v_d> mfccs = mfccComputer.process_buffer(wavFp);
        
        if(mfccs.size() == 0)
        {
            std::cerr << "Error processing " << line.first << std::endl;
            return -1;
        }
        
        std::vector<v_d> deltas;
        std::vector<v_d> delta_deltas;
        
        deltas.resize(mfccs.size());
        delta_deltas.resize(mfccs.size());
        
        // Extract delta of MFCC
        compute_delta(mfccs, DELTA_N, deltas);
        
        // Extract delta-delta of MFCC
        compute_delta(deltas, DELTA_N, delta_deltas);
        
        if(rescale_val == 1)
        {
            rescale(mfccs);
            rescale(deltas);
            rescale(delta_deltas);
        }
        
        std::cout << "Processed File : " << line.first << " | Progress " << item_id << "/" << lines.size() << " files" << std::endl;
        
        if(verbose)
        {
            print_vector(mfccs, "MFCC");
            print_vector(deltas, "MFCC Delta");
            print_vector(delta_deltas, "MFCC Delta-Delta");
        }
        
        int subframe = 0;

        // Write to Datum
        for(uint32_t start_idx = 0; (start_idx + IMAGE_WIDTH) < mfccs.size(); start_idx += IMAGE_WIDTH)
        {
            // Clear previous data
            datum.clear_data();
            datum.clear_float_data();
            
            // Set emotion label
            datum.set_label(line.second);
            datum.set_encoded(false);
            
            value.clear();
            
            string key_str = caffe::format_int(item_id, 8) + "-" + caffe::format_int(subframe, 8);
            
            // For each coefficient
            for(uint32_t j = 1; j < (num_coefs + 1); j++)
            {
                // For each frame in Datum
                for(uint32_t frame_idx = 0; frame_idx < IMAGE_WIDTH; frame_idx++)
                {
                    datum.add_float_data(static_cast<float>(mfccs[start_idx + frame_idx][j]));
                }
            }
            
            // For each coefficient
            for(uint32_t j = 1; j < (num_coefs + 1); j++)
            {
                // For each frame in Datum
                for(uint32_t frame_idx = 0; frame_idx < IMAGE_WIDTH; frame_idx++)
                {
                    datum.add_float_data(static_cast<float>(deltas[start_idx + frame_idx][j]));
                }
            }
            
            // For each coefficient
            for(uint32_t j = 1; j < (num_coefs + 1); j++)
            {
                // For each frame in Datum
                for(uint32_t frame_idx = 0; frame_idx < IMAGE_WIDTH; frame_idx++)
                {
                    datum.add_float_data(static_cast<float>(delta_deltas[start_idx + frame_idx][j]));
                }
            }
            
            datum.SerializeToString(&value);
            txn->Put(key_str, value);
            subframe++;
            subitems++;
        }
        
        wavFp.close();
        
        if (++item_id % 50 == 0)
        {
            txn->Commit();
            txn.reset(db->NewTransaction());
        }
    }
    
    if (item_id % 50 != 0)
        txn->Commit();
    
    std::cout << "DONE. Processed " << subitems << " items in " << item_id << " files." << std::endl;
    db->Close();
    
    return 0;
}

int main(int argc, char * argv[])
{
    std::string USAGE = "convert_audioset : lmdb audio dataset converter\n";
    USAGE += "OPTIONS\n";
    USAGE += "--numcepstra      : Number of output cepstra, excluding log-energy (default=12)\n";
    USAGE += "--numfilters      : Number of Mel warped filters in filterbank (default=40)\n";
    USAGE += "--samplingrate    : Sampling rate in Hertz (default=16000)\n";
    USAGE += "--winlength       : Length of analysis window in milliseconds (default=20)\n";
    USAGE += "--frameshift      : Frame shift in milliseconds (default=10)\n";
    USAGE += "--lowfreq         : Filterbank low frequency cutoff in Hertz (default=20)\n";
    USAGE += "--highfreq        : Filterbank high freqency cutoff in Hertz (default=samplingrate/2)\n";
    USAGE += "--dataset         : Path to the root folder of the dataset\n";
    USAGE += "--phase           : The phase to which the dataset belongs to (test/train)\n";
    USAGE += "--rescale         : Rescale each coefficient and its' time derivatives (default=0)\n";
    USAGE += "--dbpath          : Path to the output database\n";
    USAGE += "--labeltype       : The label type to use (default=0)\n";
    USAGE += "--verbose         : Print MFCC output (default=0)\n";
    USAGE += "USAGE EXAMPLES\n";
    USAGE += "./convert_audioset --dataset RAVDESS --labeltype 0 --numcepstra 12 --phase train --samplingrate 48000\n";
    USAGE += "./convert_audioset --dataset EMODB --labeltype 1 --numcepstra 12 --phase train --samplingrate 16000\n";
    
    char *num_cepstra_arg = getCmdOption(argv, argv+argc, "--numcepstra");
    char *num_filters_arg = getCmdOption(argv, argv+argc, "--numfilters");
    char *sampling_rate_arg = getCmdOption(argv, argv+argc, "--samplingrate");
    char *win_length_arg = getCmdOption(argv, argv+argc, "--winlength");
    char *frame_shift_arg = getCmdOption(argv, argv+argc, "--frameshift");
    char *low_freq_arg = getCmdOption(argv, argv+argc, "--lowfreq");
    char *high_freq_arg = getCmdOption(argv, argv+argc, "--highfreq");
    char *dataset_arg = getCmdOption(argv, argv+argc, "--dataset");
    char *phase_arg = getCmdOption(argv, argv+argc, "--phase");
    char *dbpath_arg = getCmdOption(argv, argv+argc, "--dbpath");
    char *labeltype_arg = getCmdOption(argv, argv+argc, "--labeltype");
    char *verbose_arg = getCmdOption(argv, argv+argc, "--verbose");
    char *rescale_arg = getCmdOption(argv, argv+argc, "--rescale");
    
//    const char *num_cepstra_arg = "12";
//    const char *num_filters_arg = "40";
//    const char *sampling_rate_arg = "16000";
//    const char *win_length_arg = "40";
//    const char *frame_shift_arg = "20";
//    const char *low_freq_arg = "300";
//    const char *high_freq_arg = "8000";
//    const char *dataset_arg = "../../../../datasets/speech/EMODB";
//    const char *dbpath_arg = "db";
//    const char *labeltype_arg = "1";
//    const char *verbose_arg = "0";
//    const char *rescale_arg = "1";
//    const char *phase_arg = "train";
    
    // Check arguments
    if(argc < 3 || !dataset_arg || !dbpath_arg || !phase_arg)
    {
        std::cout << "ERROR: Incorrect arguments.\n";
        std::cout << USAGE;
        return 1;
    }
    
    // Assign variables
    int num_cepstra = (num_cepstra_arg ? atoi(num_cepstra_arg) : 12);
    int num_filters = (num_filters_arg ? atoi(num_filters_arg) : 40);
    int sampling_rate = (sampling_rate_arg ? atoi(sampling_rate_arg) : 48000);
    int win_length = (win_length_arg ? atoi(win_length_arg) : 20);
    int frame_shift = (frame_shift_arg ? atoi(frame_shift_arg) : 10);
    int low_freq = (low_freq_arg ? atoi(low_freq_arg) : 20);
    int high_freq = (high_freq_arg ? atoi(high_freq_arg) : sampling_rate/2);
    int labeltype = (labeltype_arg ? atoi(labeltype_arg) : 0);
    int verbose = (verbose_arg ? atoi(verbose_arg) : 0);
    int rescale = (rescale_arg ? atoi(rescale_arg) : 0);
    
    // Initialise MFCC class instance
    MFCC mfcc_computer (sampling_rate, num_cepstra, win_length, frame_shift, num_filters, low_freq, high_freq);
    
    if(rescale == 1)
        find_min_max(mfcc_computer, dataset_arg, labeltype, num_cepstra);
    
    return process_dataset(mfcc_computer, dataset_arg, phase_arg, dbpath_arg, labeltype, verbose, num_cepstra, rescale);
}
