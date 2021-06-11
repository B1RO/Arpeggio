cmake_minimum_required(VERSION 3.4)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)

find_package(OpenCV REQUIRED)

project(key-recognition)

include_directories( ${OpenCV_INCLUDE_DIRS} )

set(key-recognition_SOURCES
        main.cpp)
add_executable(key-recognition ${key-recognition_SOURCES})
target_link_libraries (key-recognition ${OpenCV_LIBS})