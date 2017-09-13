cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

# -----------------------------------------------------------------------------------------------------------------------------
# FLAGS
# -----------------------------------------------------------------------------------------------------------------------------

if(NOT WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -stdlib=libc++")
endif()

# -----------------------------------------------------------------------------------------------------------------------------
# FILES
# -----------------------------------------------------------------------------------------------------------------------------

# Headers
set(EMO_GPU_HEADERS "${PROJECT_SOURCE_DIR}/include/classifier.hpp"
                    "${PROJECT_SOURCE_DIR}/include/facial_emotion_classifier.hpp"
                    "${PROJECT_SOURCE_DIR}/include/facial_emotion_classifier_cpu.hpp"
                    "${PROJECT_SOURCE_DIR}/include/facial_emotion_classifier_gpu.hpp"
                    "${PROJECT_SOURCE_DIR}/include/speech_emotion_classifier.hpp"
                    "${PROJECT_SOURCE_DIR}/include/speech_emotion_classifier_cpu.hpp"
                    "${PROJECT_SOURCE_DIR}/include/speech_emotion_classifier_gpu.hpp"
                    "${PROJECT_SOURCE_DIR}/include/trainer.hpp"
                    "${PROJECT_SOURCE_DIR}/include/common.hpp"
                    "${PROJECT_SOURCE_DIR}/include/macros.hpp")

# Sources
set(EMO_GPU_SOURCES "facial_emotion_classifier.cpp")

# Source groups
source_group("Headers" FILES ${EMO_GPU_HEADERS})
source_group("Sources" FILES ${EMO_GPU_SOURCES})

# -----------------------------------------------------------------------------------------------------------------------------
# TARGETS
# -----------------------------------------------------------------------------------------------------------------------------

add_library(EmoGPU ${EMO_GPU_HEADERS} ${EMO_GPU_SOURCES})

# -----------------------------------------------------------------------------------------------------------------------------
# LINKING
# -----------------------------------------------------------------------------------------------------------------------------

target_link_libraries(EmoGPU caffe)
target_link_libraries(EmoGPU sndfile-static)
target_link_libraries(EmoGPU LibXtract)