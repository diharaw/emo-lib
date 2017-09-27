#include <macros.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

EMOLIB_BEGIN_NAMESPACE

struct InputImage
{
    InputImage();
    ~InputImage();
    
    cv::Mat m_cv_img;
    char*   m_data;
};

EMOLIB_END_NAMESPACE
