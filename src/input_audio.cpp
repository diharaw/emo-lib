#include <input_audio.hpp>

EMOLIB_BEGIN_NAMESPACE

InputAudio::InputAudio()
{
    
}

InputAudio::~InputAudio()
{
    if(m_data)
        delete [] m_data;
}

EMOLIB_END_NAMESPACE
