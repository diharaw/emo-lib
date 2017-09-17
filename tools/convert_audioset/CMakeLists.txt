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

# Sources
set(CONVERT_AUDIOSET_SOURCES "main.cpp" "mfcc.cpp")

# Headers
set(CONVERT_AUDIOSET_HEADERS "wav_header.h")

# Source groups
source_group("Sources" FILES ${CONVERT_AUDIOSET_HEADERS} ${CONVERT_AUDIOSET_SOURCES})

# -----------------------------------------------------------------------------------------------------------------------------
# TARGETS
# -----------------------------------------------------------------------------------------------------------------------------

add_executable(convert_audioset ${CONVERT_AUDIOSET_HEADERS} ${CONVERT_AUDIOSET_SOURCES})

# -----------------------------------------------------------------------------------------------------------------------------
# LINKING
# -----------------------------------------------------------------------------------------------------------------------------

target_link_libraries(convert_audioset caffe)
target_link_libraries(convert_audioset sndfile-static)
target_link_libraries(convert_audioset LibXtract)