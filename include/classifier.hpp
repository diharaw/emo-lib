#pragma once

#include <common.hpp>

EMOLIB_BEGIN_NAMESPACE

class Classifier
{
public:
    Classifier();
    ~Classifier();
    void     load_facial_model(const char* model, const char* weights);
    void     load_speech_model(const char* model, const char* weights);
    uint32_t classify(char* img, char* audio);
};

EMOLIB_END_NAMESPACE
