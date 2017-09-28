#include <common.hpp>
#include <string>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

EMOLIB_BEGIN_NAMESPACE

struct InputImage;

class InputImageBuilder
{
public:
    InputImageBuilder();
    bool initialize(std::string face_det_1,
                    std::string face_det_2,
                    std::string face_det_3,
                    std::string face_det_4);
    cv::Mat crop_face(std::vector<cv::Rect>& faces, cv::Mat img);
    bool crop_face(cv::Mat img, cv::Mat& out);
    InputImage* build(char* data, uint32_t width, uint32_t height);
    InputImage* build(std::string file);
    InputImage* build(cv::Mat cv_img);
    
private:
    cv::CascadeClassifier m_face_det_1;
    cv::CascadeClassifier m_face_det_2;
    cv::CascadeClassifier m_face_det_3;
    cv::CascadeClassifier m_face_det_4;
};

EMOLIB_END_NAMESPACE
