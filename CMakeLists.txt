cmake_minimum_required(VERSION 3.5)

project(Paint LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(cmake-build-debug/CMakeFiles/Paint_autogen.dir/include)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(Paint
        src/main.cpp
        src/PaintWindow/PaintWindow.cpp
        src/PaintWindow/PaintWindow.hpp
        src/PaintWindow/PaintWindow.ui
        src/PaintWidget/PaintWidget.cpp src/PaintWidget/PaintWidget.hpp src/model/Tools.hpp src/model/Drawable.hpp)

target_link_libraries(Paint PRIVATE Qt5::Widgets)
