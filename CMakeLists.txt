cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

# -----------------------------------------------------------------------------------------------------------------------------
# PROJECT
# -----------------------------------------------------------------------------------------------------------------------------

project("EmoLib")

# -----------------------------------------------------------------------------------------------------------------------------
# OPTIONS
# -----------------------------------------------------------------------------------------------------------------------------

set(BUILD_GUI_TOOL ON CACHE BOOL "Specify whether the Qt based GUI Tool should be built.")
set(BUILD_AUDIOSET_CONVERT_TOOL ON CACHE BOOL "Specify whether the audio dataset conversion tool should be built.")
set(BUILD_CMD_TOOL ON CACHE BOOL "Specify whether the command line tool should be built.")

# -----------------------------------------------------------------------------------------------------------------------------
# OUTPUT DIRECTORIES
# -----------------------------------------------------------------------------------------------------------------------------                    

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# -----------------------------------------------------------------------------------------------------------------------------
# SUB-DIRECTORIES
# -----------------------------------------------------------------------------------------------------------------------------                    

# Dependencies
add_subdirectory("external/caffe")
add_subdirectory("external/libsndfile")
add_subdirectory("external/LibXtract")

# -----------------------------------------------------------------------------------------------------------------------------
# INCLUDES
# -----------------------------------------------------------------------------------------------------------------------------

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)
include(cmake/Dependencies.cmake)

# Setup library include paths
set(EMO_LIB_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/include")

# Setup external include paths
set(CAFFE_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/external/caffe/include/caffe")
set(CAFFE_INCLUDE_PATH_INTERNAL "${PROJECT_SOURCE_DIR}/external/caffe/include")
set(CAFFE_INCLUDE_PATH_GENERATED "${PROJECT_SOURCE_DIR}/build/external/caffe/include")
set(LIBSNDFILE_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/external/libsndfile/src")
set(LIBXTRACT_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/external/LibXtract/include")

# Set includes
include_directories("${EMO_LIB_INCLUDE_PATH}"
                    "${CAFFE_INCLUDE_PATH}"
                    "${CAFFE_INCLUDE_PATH_INTERNAL}"
                    "${CAFFE_INCLUDE_PATH_GENERATED}"
                    "${LIBSNDFILE_INCLUDE_PATH}"
                    "${LIBXTRACT_INCLUDE_PATH}"
                    "${ViennaCL_INCLUDE_DIRS}"
                    "${LMDB_INCLUDE_DIR}"
                    "${LevelDB_INCLUDES}"
                    "${Boost_INCLUDE_DIRS}")

# -----------------------------------------------------------------------------------------------------------------------------
# PROJECTS
# ----------------------------------------------------------------------------------------------------------------------------- 

# Library
add_subdirectory("src")

# Tools
if(BUILD_GUI_TOOL)
    # add_subdirectory("tools/gui_tool")
endif()

if(BUILD_AUDIOSET_CONVERT_TOOL)
    add_subdirectory("tools/convert_audioset")
endif()

if(BUILD_CMD_TOOL)
    add_subdirectory("tools/cmd_tool")
endif()


