#pragma once

#include <common.hpp>
#include <string>

EMOLIB_BEGIN_NAMESPACE

struct InputAudio;
struct InputImage;
class CaffeClassifier;

class Classifier
{
public:
    Classifier();
    ~Classifier();
    void     load_facial_model(std::string model, std::string weights, std::string mean, std::string label);
    void     load_speech_model(std::string model, std::string weights, std::string mean, std::string label);
    int32_t classify(InputImage* img, InputAudio* audio);
    
private:
    CaffeClassifier* m_facial_classifier;
    CaffeClassifier* m_speech_classifier;
};

EMOLIB_END_NAMESPACE
