#include <input_audio_builder.hpp>
#include <input_audio.hpp>
#include <mfcc_computer.hpp>

#define AUDIO_IMAGE_HEIGHT 36
#define AUDIO_IMAGE_WIDTH 36
#define DELTA_N 2

EMOLIB_BEGIN_NAMESPACE

static void compute_delta(std::vector<v_d>& input, uint32_t n, std::vector<v_d>& output)
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

InputAudioBuilder::InputAudioBuilder()
{
    
}

InputAudioBuilder::~InputAudioBuilder()
{
    if(m_mfcc_computer)
        delete m_mfcc_computer;
}

bool InputAudioBuilder::initialize(int sample_rate, int window_size, int hop_size)
{
    m_sample_rate = sample_rate;
    m_window_size = window_size;
    m_hop_size = hop_size;
    
    if(m_mfcc_computer)
        delete m_mfcc_computer;
    
    m_mfcc_computer = new MFCC(m_sample_rate, 12, m_window_size, m_hop_size);
    
    return true;
}

uint64_t InputAudioBuilder::num_required_samples()
{
    return (m_sample_rate / 1000) * m_window_size * AUDIO_IMAGE_WIDTH;
}

cv::Mat InputAudioBuilder::extract_features(int16_t* samples, uint64_t count)
{
    cv::Mat img(AUDIO_IMAGE_WIDTH, AUDIO_IMAGE_HEIGHT, CV_32FC1);
    std::vector<v_d> mfcc_frames = m_mfcc_computer->process_buffer(samples, count);
    
    std::vector<v_d> deltas;
    std::vector<v_d> delta_deltas;
    
    deltas.resize(mfcc_frames.size());
    delta_deltas.resize(mfcc_frames.size());
    
    // Extract delta of MFCC
    compute_delta(mfcc_frames, DELTA_N, deltas);
    
    // Extract delta-delta of MFCC
    compute_delta(deltas, DELTA_N, delta_deltas);
    
    for(int i = 0; i < 12; i++)
    {
        for(int j = 0; j < AUDIO_IMAGE_WIDTH; j++)
        {
//            img.at<float>(i, j) =
        }
    }
    
    for(int i = 12; i < 23; i++)
    {
        for(int j = 0; j < AUDIO_IMAGE_WIDTH; j++)
        {
            //            img.at<float>(i, j) =
        }
    }
    
    for(int i = 23; i < 35; i++)
    {
        for(int j = 0; j < AUDIO_IMAGE_WIDTH; j++)
        {
            //            img.at<float>(i, j) =
        }
    }
    
    return img;
}

InputAudio* InputAudioBuilder::build(int16_t* samples, uint64_t count)
{
    InputAudio* audio = new InputAudio();
    audio->m_img = extract_features(samples, count);
    audio->m_data = samples;
    return audio;
}

EMOLIB_END_NAMESPACE

