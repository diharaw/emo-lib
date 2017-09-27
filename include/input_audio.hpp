#include <common.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

EMOLIB_BEGIN_NAMESPACE

struct InputAudio
{
    InputAudio();
    ~InputAudio();
    
    cv::Mat   m_img;
    int16_t* m_data;
};

EMOLIB_END_NAMESPACE
