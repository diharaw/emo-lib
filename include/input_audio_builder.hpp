#include <common.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

EMOLIB_BEGIN_NAMESPACE

class MFCC;
struct InputAudio;

class InputAudioBuilder
{
public:
    InputAudioBuilder();
    ~InputAudioBuilder();
    uint64_t num_required_samples();
    bool initialize(int sample_rate, int window_size, int hop_size);
    cv::Mat extract_features(int16_t* samples, uint64_t count);
    InputAudio* build(int16_t* samples, uint64_t count);
    
private:
    MFCC* m_mfcc_computer;
    int m_sample_rate;
    int m_window_size;
    int m_hop_size;
};

EMOLIB_END_NAMESPACE
