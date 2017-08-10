#pragma once

#include <macros.hpp>

EMOGPU_BEGIN_NAMESPACE

class IClassifier
{
public:
    virtual bool deserialize_model(void* data, size_t size) = 0;
    virtual void serialize_model(void* data) = 0;
    virtual size_t model_size() = 0;
};

EMOGPU_END_NAMESPACE
