#include <iostream>
#include <string>
#include <dirent.h>
#include <xtract/libxtract.h>

static const char* g_emotions_a[] =
{
    "neutral",
    "anger",
    "fear",
    "happy",
    "sadness",
    "surprise"
};

static const char* g_emotions_b[] =
{
    "neutral",
    "anger",
    "fear",
    "happy",
    "sadness",
    "disgust"
};

static const char** g_label_types[] =
{
    g_emotions_a,
    g_emotions_b
};

int main(int argc, const char * argv[])
{
    // Path to dataset root.
    if(argc != 3)
    {
        std::cout << "usage: convert_imageset <path_to_dataset_root> <label_type>" << std::endl;
        return -1;
    }
    
    std::string dataset_root = std::string(argv[1]);
    int label_type = atoi(argv[2]);
    
    std::string full_path = dataset_root + "/2 - PARTITIONED/train/";
    
    for(uint32_t i = 0; i < 6; i++)
    {
        std::string path_with_emotion = full_path + std::string(g_label_types[label_type][i]);
        
        std::cout << std::endl;
        std::cout << "Processing Emotion : " << g_label_types[label_type][i] << std::endl;
        std::cout << std::endl;
        
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir (path_with_emotion.c_str())) != NULL)
        {
            /* print all the files and directories within directory */
            while ((ent = readdir (dir)) != NULL)
            {
                std::string file = std::string(ent->d_name);
                
                if(file.length() > 2)
                    std::cout << file << std::endl;
            }
            closedir (dir);
        }
        else
        {
            /* could not open directory */
            perror ("");
            return EXIT_FAILURE;
        }
    }
    
    return 0;
}
