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
set(EMO_LIB_HEADERS "${PROJECT_SOURCE_DIR}/include/classifier.hpp"
                    "${PROJECT_SOURCE_DIR}/include/input_audio_builder.hpp"
                    "${PROJECT_SOURCE_DIR}/include/input_image_builder.hpp"
                    "${PROJECT_SOURCE_DIR}/include/input_audio.hpp"
                    "${PROJECT_SOURCE_DIR}/include/input_image.hpp"
                    "${PROJECT_SOURCE_DIR}/include/mfcc_computer.hpp"
                    "${PROJECT_SOURCE_DIR}/include/caffe_classifier.hpp"
                    "${PROJECT_SOURCE_DIR}/include/memory_stream.hpp"
                    "${PROJECT_SOURCE_DIR}/include/common.hpp"
                    "${PROJECT_SOURCE_DIR}/include/macros.hpp")

# Sources
set(EMO_LIB_SOURCES "classifier.cpp"
                    "input_audio_builder.cpp"
                    "caffe_classifier.cpp"
                    "input_audio.cpp"
                    "input_image_builder.cpp"
                    "memory_stream.cpp"
                    "input_image.cpp"
                    "mfcc_computer.cpp")

# Source groups
source_group("Headers" FILES ${EMO_LIB_HEADERS})
source_group("Sources" FILES ${EMO_LIB_SOURCES})

# -----------------------------------------------------------------------------------------------------------------------------
# TARGETS
# -----------------------------------------------------------------------------------------------------------------------------

add_library(EmoLib ${EMO_LIB_HEADERS} ${EMO_LIB_SOURCES})

# -----------------------------------------------------------------------------------------------------------------------------
# LINKING
# -----------------------------------------------------------------------------------------------------------------------------

target_link_libraries(EmoLib caffe)
target_link_libraries(EmoLib sndfile-static)
target_link_libraries(EmoLib LibXtract)