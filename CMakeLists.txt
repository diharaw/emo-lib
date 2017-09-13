cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

# -----------------------------------------------------------------------------------------------------------------------------
# PROJECT
# -----------------------------------------------------------------------------------------------------------------------------

project("EmoGPU")

# -----------------------------------------------------------------------------------------------------------------------------
# INCLUDES
# -----------------------------------------------------------------------------------------------------------------------------

# Setup library include paths
set(EMO_GPU_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/include")

# Setup external include paths
set(CAFFE_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/external/caffe/include/caffe")
set(LIBSNDFILE_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/external/libsndfile/src")
set(LIBXTRACT_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/external/LibXtract/include/xtract")

# Set includes
include_directories("${EMO_GPU_INCLUDE_PATH}"
                    "${CAFFE_INCLUDE_PATH}"
                    "${LIBSNDFILE_INCLUDE_PATH}"
                    "${LIBXTRACT_INCLUDE_PATH}")

# -----------------------------------------------------------------------------------------------------------------------------
# SUB-DIRECTORIES
# -----------------------------------------------------------------------------------------------------------------------------                    

# Dependencies
add_subdirectory("external/caffe")
add_subdirectory("external/libsndfile")

# Library
add_subdirectory("src")

# Tools
add_subdirectory("tools/gui_tool")