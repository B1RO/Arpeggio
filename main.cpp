#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h> 
#include "key-recognition/KeyFinder.h"
#include "key-recognition/VideoFeedTransformer.h"
#include "rendering/Renderer.h"
#include "music/MusicSelection.h"
#include <iostream>
#include <iomanip>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;
//Camera Settings for laptop camera 
const int camera_width = 640;
const int camera_height = 480;

//Camera Settings for video sample_video_hands
//const int camera_width = 1920;
//const int camera_height = 1080;

//Camera Settings for video sample_video_1
//const int camera_width = 3840;
//const int camera_height = 2160;

void initVideoStream(cv::VideoCapture& cap) {
	auto path = "C:\\Code\\Arpeggio\\sample_video_1.mp4";
	if (cap.isOpened())
		cap.release();

	if (cap.isOpened() == false) {
		std::cout << "No webcam found, using a video file" << std::endl;
		cap.open(path);
		if (cap.isOpened() == false) {
			std::cout << "No video file found. Exiting." << std::endl;
			exit(0);
		}
	}
}

int main(int argc, char* argv[]) {
	// Select Music and Mode
	auto musicMode = MusicSelection::selectMusic();

	GLFWwindow* window;

	// Initialize the library
	if (!glfwInit())
		return -1;

	// Initialize the window system
	// Create a windowed mode window and its OpenGL context
	window = glfwCreateWindow(camera_width, camera_height, "ARpeggio", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	// Initialize the GL library
	Renderer renderer(window, get<0>(musicMode), get<1>(musicMode));
	renderer.initGL();

	cv::VideoCapture cap;
	initVideoStream(cap);
	VideoFeedTransformer transformer;
	transformer.consumeFeed(cap, Renderer::processFrame, renderer);
	
	glfwTerminate();
	return 0;
}