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
set(DEEPCL_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/external/DeepCL/src")
set(TINY_DNN_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/external/tiny-dnn/tiny-dnn")
set(LIBSNDFILE_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/external/libsndfile/src")
set(LIBXTRACT_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/external/LibXtract/include/xtract")

# Set includes
include_directories("${DEEPCL_INCLUDE_PATH}"
                    "${TINY_DNN_INCLUDE_PATH}"
                    "${LIBSNDFILE_INCLUDE_PATH}"
                    "${LIBXTRACT_INCLUDE_PATH}")

# -----------------------------------------------------------------------------------------------------------------------------
# SUB-DIRECTORIES
# -----------------------------------------------------------------------------------------------------------------------------                    

add_subdirectory("external/DeepCL")
add_subdirectory("external/libsndfile")
add_subdirectory("external/tiny-dnn")
add_subdirectory("src")
add_subdirectory("examples")