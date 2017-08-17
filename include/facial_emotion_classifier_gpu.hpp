#pragma once

#include <facial_emotion_classifier.hpp>

EMOGPU_BEGIN_NAMESPACE

class FacialEmotionClassifierGPU : public FacialEmotionClassifier
{
public:
    bool     deserialize_model(void* data, size_t size) override;
    void     serialize_model(void* data)                override;
    size_t   model_size()                               override;
    uint32_t classify(char* data, size_t size)          override;
};

EMOGPU_END_NAMESPACE
