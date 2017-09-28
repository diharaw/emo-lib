#pragma once

#include <common.hpp>
#include <caffe/caffe.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <algorithm>
#include <iosfwd>
#include <memory>
#include <string>
#include <utility>
#include <vector>

using namespace caffe;  // NOLINT(build/namespaces)
using std::string;
typedef std::pair<string, float> Prediction;
typedef std::pair<int_tp, float> PredictionIdx;

EMOLIB_BEGIN_NAMESPACE

class CaffeClassifier
{
public:
    CaffeClassifier(const string& model_file,
                    const string& trained_file,
                    const string& mean_file,
                    const string& label_file);
    
    std::vector<Prediction> Classify(const cv::Mat& img, int_tp N = 5);
    std::vector<PredictionIdx> ClassifyIndexes(const cv::Mat& img, int_tp N = 5);
    
private:
    void SetMean(const string& mean_file);
    
    std::vector<float> Predict(const cv::Mat& img);
    
    void WrapInputLayer(std::vector<cv::Mat>* input_channels);
    
    void Preprocess(const cv::Mat& img,
                    std::vector<cv::Mat>* input_channels);
    
private:
    shared_ptr<Net<float> > net_;
    cv::Size input_geometry_;
    int_tp num_channels_;
    cv::Mat mean_;
    std::vector<string> labels_;
};

EMOLIB_END_NAMESPACE
