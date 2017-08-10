#pragma once

#include <classifier.hpp>
#include <tiny_dnn.h>

EMOGPU_BEGIN_NAMESPACE

class FacialEmotionClassifier : public IClassifier
{
public:
    // Expects a char array of size 32 * 32
    virtual double* classify(char* data) = 0;
};

EMOGPU_END_NAMESPACE

