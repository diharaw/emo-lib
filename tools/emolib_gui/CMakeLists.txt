cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

# -----------------------------------------------------------------------------------------------------------------------------
# FLAGS
# -----------------------------------------------------------------------------------------------------------------------------

if(NOT WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -stdlib=libc++")
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set (CMAKE_PREFIX_PATH "/Users/diharaw/Qt/5.9.1/clang_64/lib/cmake")

find_package(Qt5Widgets)
find_package(Qt5Gui)
find_package(Qt5Multimedia)
find_package(Qt5MultimediaWidgets)
find_package(Qt5Charts)

# -----------------------------------------------------------------------------------------------------------------------------
# FILES
# -----------------------------------------------------------------------------------------------------------------------------

# Headers
set(GUI_TOOL_HEADERS "mainwindow.h"
                    "wavfile.h"
                    "xyseriesiodevice.h"
                    "utils.h")

# Sources
set(GUI_TOOL_SOURCES "main.cpp"
                     "mainwindow.cpp"
                     "wavfile.cpp"
                     "xyseriesiodevice.cpp")

SET(GUI_TOOL_FORMS mainwindow.ui)

QT5_WRAP_CPP(APP_HEADERS_MOC "mainwindow.h")
QT5_WRAP_UI(APP_FORMS_HEADERS ${GUI_TOOL_FORMS})

# Source groups
source_group("Headers" FILES ${GUI_TOOL_HEADERS})
source_group("Sources" FILES ${GUI_TOOL_SOURCES})

# -----------------------------------------------------------------------------------------------------------------------------
# TARGETS
# -----------------------------------------------------------------------------------------------------------------------------

if(APPLE)
    add_executable(gui_tool MACOSX_BUNDLE ${GUI_TOOL_HEADERS} ${GUI_TOOL_SOURCES})
else()
    add_executable(gui_tool ${GUI_TOOL_HEADERS} ${GUI_TOOL_SOURCES})
endif()

# -----------------------------------------------------------------------------------------------------------------------------
# LINKING
# -----------------------------------------------------------------------------------------------------------------------------

target_link_libraries(gui_tool EmoLib)
target_link_libraries(gui_tool ${OpenCV_LIBS})
target_link_libraries(gui_tool Qt5::Widgets)
target_link_libraries(gui_tool Qt5::Gui)
target_link_libraries(gui_tool Qt5::Charts)
target_link_libraries(gui_tool Qt5::Multimedia)
target_link_libraries(gui_tool Qt5::MultimediaWidgets)

if(APPLE)
    target_link_libraries(gui_tool "-framework CoreFoundation")
endif()