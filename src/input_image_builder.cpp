#include <input_image_builder.hpp>
#include <iostream>
#include <input_image.hpp>
#include <vector>

#define IMAGE_WIDTH 224
#define IMAGE_HEIGHT 224

EMOLIB_BEGIN_NAMESPACE

InputImageBuilder::InputImageBuilder()
{
    
}

bool InputImageBuilder::initialize(std::string face_det_1,
                                   std::string face_det_2,
                                   std::string face_det_3,
                                   std::string face_det_4)
{
    if (!m_face_det_1.load(face_det_1))
    {
        std::cout << "ERROR: Failed to load Haar-Cascade filter - " << face_det_1 << std::endl;
        return false;
    }
    
    if (!m_face_det_2.load(face_det_2))
    {
        std::cout << "ERROR: Failed to load Haar-Cascade filter - " << face_det_1 << std::endl;
        return false;
    }
    
    if (!m_face_det_3.load(face_det_3))
    {
        std::cout << "ERROR: Failed to load Haar-Cascade filter - " << face_det_1 << std::endl;
        return false;
    }
    
    if (!m_face_det_4.load(face_det_4))
    {
        std::cout << "ERROR: Failed to load Haar-Cascade filter - " << face_det_1 << std::endl;
        return false;
    }
    
    return true;
}

cv::Mat InputImageBuilder::crop_face(std::vector<cv::Rect>& faces, cv::Mat img)
{
    cv::Mat cropped;
    cv::Mat resized;
    cv::Rect roi_b;
    cv::Rect roi_c;
    
    size_t ic = 0;
    int ac = 0;
    
    size_t ib = 0;
    int ab = 0;
    
    for (ic = 0; ic < faces.size(); ic++)
    {
        roi_c.x = faces[ic].x;
        roi_c.y = faces[ic].y;
        roi_c.width = (faces[ic].width);
        roi_c.height = (faces[ic].height);
        
        ac = roi_c.width * roi_c.height;
        
        roi_b.x = faces[ib].x;
        roi_b.y = faces[ib].y;
        roi_b.width = (faces[ib].width);
        roi_b.height = (faces[ib].height);
        
        ab = roi_b.width * roi_b.height;
        
        if (ac > ab)
        {
            ib = ic;
            roi_b.x = faces[ib].x;
            roi_b.y = faces[ib].y;
            roi_b.width = (faces[ib].width);
            roi_b.height = (faces[ib].height);
        }
        
        cropped = img(roi_b);
        cv::resize(cropped, resized, cv::Size(IMAGE_WIDTH, IMAGE_HEIGHT), 0, 0, cv::INTER_LINEAR);
        
        break;
    }
    
    return resized;
}

bool InputImageBuilder::crop_face(cv::Mat img, cv::Mat& out)
{
    std::vector<cv::Rect> faces_1;
    std::vector<cv::Rect> faces_2;
    std::vector<cv::Rect> faces_3;
    std::vector<cv::Rect> faces_4;
    
    m_face_det_1.detectMultiScale(img, faces_1, 1.1, 10, cv::CASCADE_SCALE_IMAGE);
    m_face_det_2.detectMultiScale(img, faces_2, 1.1, 10, cv::CASCADE_SCALE_IMAGE);
    m_face_det_3.detectMultiScale(img, faces_3, 1.1, 10, cv::CASCADE_SCALE_IMAGE);
    m_face_det_4.detectMultiScale(img, faces_4, 1.1, 10, cv::CASCADE_SCALE_IMAGE);

    if(faces_1.size() > 0)
        out = crop_face(faces_1, img);
    else if(faces_2.size() > 0)
        out = crop_face(faces_2, img);
    else if(faces_3.size() > 0)
        out = crop_face(faces_3, img);
    else if(faces_4.size() > 0)
        out = crop_face(faces_4, img);
    else
        return false;
    
    return true;
}

InputImage* InputImageBuilder::build(std::string file)
{
    InputImage* img = new InputImage();
    cv::Mat temp = cv::imread(file);
    
    if(!crop_face(temp, img->m_cv_img))
        return nullptr;
    
    img->m_data = nullptr;
    return img;
}

InputImage* InputImageBuilder::build(char* data, uint32_t width, uint32_t height)
{
    InputImage* img = new InputImage();
    cv::Mat temp = cv::Mat(height, width, CV_8SC3, data);
    
    if(!crop_face(temp, img->m_cv_img))
        return nullptr;
    
    img->m_data = data;
    return img;
}

EMOLIB_END_NAMESPACE
