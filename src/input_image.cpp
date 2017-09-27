#include <input_image.hpp>

EMOLIB_BEGIN_NAMESPACE

InputImage::InputImage()
{
    
}

InputImage::~InputImage()
{
    if(m_data)
        delete [] m_data;
}

EMOLIB_END_NAMESPACE
