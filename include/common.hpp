#pragma once

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
