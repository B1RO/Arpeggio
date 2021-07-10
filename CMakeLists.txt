cmake_minimum_required(VERSION 3.19)
project(Arpeggio)
find_package( OpenCV REQUIRED PATHS "C:\\Users\\Miruna\\OpenCV_3_4_14\\opencv\\build" )
set(CMAKE_CXX_STANDARD 20)


find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
if (GLEW_FOUND)
include_directories( ${GLEW_INCLUDE_DIRS} "C:\\Users\\Miruna\\Libraries\\glfw-3.3.4.bin.WIN64\\include" ${OpenCV_INCLUDE_DIRS})
add_executable(Arpeggio main.cpp music/Pitch.h music/CBlues.h music/CBlues.cpp music/CDur.h music/CDur.cpp music/ContinuousMusic.h music/ContinuousMusic.cpp  music/Music.h music/Music.cpp rendering/Renderer.h rendering/Renderer.cpp  key-recognition/KeyFinder.cpp key-recognition/KeyFinder.h key-recognition/VideoFeedTransformer.cpp key-recognition/VideoFeedTransformer.h key-recognition/CMarkerFinder.cpp key-recognition/CMarkerFinder.h key-recognition/ContourScaler.h)

target_link_libraries(Arpeggio ${GLEW_LIBRARIES} "C:\\Users\\Miruna\\Libraries\\glfw-3.3.4.bin.WIN64\\lib-vc2019\\glfw3.lib" ${OPENGL_LIBRARIES} ${OpenCV_LIBS})
endif()