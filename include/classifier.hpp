#pragma once

#include <common.hpp>
#include <string>
#include <vector>

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
    std::vector<float> classify_vec(InputImage* img, InputAudio* audio);
    int32_t classify(InputImage* img, InputAudio* audio);
    
private:
    CaffeClassifier* m_facial_classifier = nullptr;
    CaffeClassifier* m_speech_classifier = nullptr;
};

EMOLIB_END_NAMESPACE
