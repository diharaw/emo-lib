#include <iostream>
#include <string>
#include <classifier.hpp>
#include <input_image_builder.hpp>
#include <input_image.hpp>

#define FILTER_1 "/Users/diharaw/Documents/Personal/EmoGPU/filters/haarcascade_frontalface_default.xml"
#define FILTER_2 "/Users/diharaw/Documents/Personal/EmoGPU/filters/haarcascade_frontalface_alt2.xml"
#define FILTER_3 "/Users/diharaw/Documents/Personal/EmoGPU/filters/haarcascade_frontalface_alt.xml"
#define FILTER_4 "/Users/diharaw/Documents/Personal/EmoGPU/filters/haarcascade_frontalface_alt_tree.xml"

int main(int argc, char *argv[])
{
    if(argc < 6)
    {
        std::cout << "Error: Incorrect number of arguments" << std::endl;
        std::cout << "usage: cmd_tool <model> <weights> <mean> <label> <image>" << std::endl;
        return 1;
    }
    
    std::string model = std::string(argv[1]);
    std::string weights = std::string(argv[2]);
    std::string mean = std::string(argv[3]);
    std::string label = std::string(argv[4]);
    std::string image = std::string(argv[5]);
    
    emolib::Classifier classifier;
    emolib::InputImageBuilder builder;
    
    if(!builder.initialize(FILTER_1, FILTER_2, FILTER_3, FILTER_4))
    {
        std::cout << "Failed to initialize Input Builder" << std::endl;
        return 1;
    }
    
    classifier.load_facial_model(model, weights, mean, label);
    
    emolib::InputImage* input = builder.build(image);
    
    int result = classifier.classify(input, nullptr);
    
    std::cout << "Classification for " << image << std::endl;
    std::cout << "Result : " << emolib::EMOTION_LABELS[result] << " (" << std::to_string(result) << ")" << std::endl;
    
    return 0;
}
