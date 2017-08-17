#pragma once

#include <macros.hpp>
#include <tiny-dnn.hpp>

EMOGPU_BEGIN_NAMESPACE

// Disgust is omitted due to being too similar to Anger.

enum EmotionType
{
    EMOTION_NEUTRAL  = 0,
    EMOTION_ANGRY    = 1,
    EMOTION_FEARFUL  = 2,
    EMOTION_HAPPY    = 3,
    EMOTION_SAD      = 4,
    EMOTION_SURPRISE = 5
};

static const char* EMOTION_LABELS[] = { "neutral", "angry", "fearful", "happy", "sad", "surprise" };

extern void make_vgg16_cpu_facial();
extern void make_vgg16_cpu_speech();

extern void make_alex_net_cpu_facial();
extern void make_alex_net_cpu_speech();

extern void make_vgg16_gpu_facial();
extern void make_vgg16_gpu_speech();

extern void make_alex_net_gpu_facial();
extern void make_alex_net_gpu_speech();

EMOGPU_END_NAMESPACE
