#include <classifier.hpp>
#include <input_audio.hpp>
#include <input_image.hpp>
#include <caffe_classifier.hpp>
#include <limits.h>

EMOLIB_BEGIN_NAMESPACE

#define FACIAL_MODEL_ACCURACY 98.2f
#define SPEECH_MODEL_ACCURACY 67.0f

Classifier::Classifier()
{
    
}

Classifier::~Classifier()
{
    if(m_facial_classifier)
        delete m_facial_classifier;
    
    if(m_speech_classifier)
        delete m_speech_classifier;
}

void Classifier::load_facial_model(std::string model, std::string weights, std::string mean, std::string label)
{
    m_facial_classifier = new CaffeClassifier(model, weights, mean, label);
}

void Classifier::load_speech_model(std::string model, std::string weights, std::string mean, std::string label)
{
    m_speech_classifier = new CaffeClassifier(model, weights, mean, label);
}

static int32_t max_class(std::vector<PredictionIdx>& predictions)
{
    float max = predictions[0].second;
    int32_t maxIdx = predictions[0].first;
    
    for(auto& pair : predictions)
    {
        if(pair.second > max)
            maxIdx = pair.first;
    }
    
    return max;
}

int32_t Classifier::classify(InputImage* img, InputAudio* audio)
{
    std::vector<PredictionIdx> facial_predictions;
    std::vector<PredictionIdx> speech_predictions;
    
    if(img)
    {
        facial_predictions = m_facial_classifier->ClassifyIndexes(img->m_cv_img);
        delete[] img->m_data;
        delete img;
    }
    
    if(audio)
    {
        speech_predictions = m_speech_classifier->ClassifyIndexes(audio->m_img);
        delete[] audio->m_data;
        delete audio;
    }
    
    if(facial_predictions.size() > 0 && speech_predictions.size() == 0)
    {
        return max_class(facial_predictions);
    }
    else if(facial_predictions.size() == 0 && speech_predictions.size() > 0)
    {
        return max_class(speech_predictions);
    }
    else if(facial_predictions.size() > 0 && speech_predictions.size() > 0)
    {
        std::vector<PredictionIdx> bimodel_predictions;
        float percentage = 0.0f;
        float numerator = 0.0f;
        float denominator = 0.0f;
        
        if(SPEECH_MODEL_ACCURACY > FACIAL_MODEL_ACCURACY)
        {
            numerator = FACIAL_MODEL_ACCURACY;
            denominator = SPEECH_MODEL_ACCURACY;
        }
        else if(FACIAL_MODEL_ACCURACY > SPEECH_MODEL_ACCURACY)
        {
            numerator = SPEECH_MODEL_ACCURACY;
            denominator = FACIAL_MODEL_ACCURACY;
        }
        
        percentage = numerator/denominator;
        
        for(int i = 0; i < 5; i++)
        {
            float D = percentage/2.0f;
            float P1 = 0.0f;
            float P2 = 0.0f;
            
            if(SPEECH_MODEL_ACCURACY > FACIAL_MODEL_ACCURACY)
            {
                P1 = speech_predictions[i].second;
                P2 = facial_predictions[i].second;
            }
            else if(FACIAL_MODEL_ACCURACY > SPEECH_MODEL_ACCURACY)
            {
                P1 = facial_predictions[i].second;
                P2 = speech_predictions[i].second;
            }
            
            float val = ((0.5f + D) * P1) + ((0.5f - D) * P2);
            bimodel_predictions.push_back(std::make_pair(speech_predictions[i].first , val));
        }
        
        return max_class(bimodel_predictions);
    }
    else
    {
        return UINT32_MAX;
    }
}

EMOLIB_END_NAMESPACE
