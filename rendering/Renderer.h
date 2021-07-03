//
// Created by Miruna on 6/26/2021.
//
#ifndef ARPEGGIO_RENDERER_H
#define ARPEGGIO_RENDERER_H

#define GLFW_INCLUDE_GLU
#include <glfw/glfw3.h> 
#include "../key-recognition/KeyFinder.h"
#include "../key-recognition/CMarkerFinder.h"
#include "../music/Pitch.h"
#include "../music/Music.h"
#include "../music/CBlues.h"
#include "../music/CDur.h"


class Renderer {
typedef tuple<vector<float>, tuple<float, float, float, float>> NoteToRender;

public:
	Renderer(GLFWwindow* win):window(win){}
	//OpenGL initialization function
	void initGL();

	//function that renders in the current frame
	void render(vector<NoteToRender> notesToRender, Mat& frame);
	//reshaping function for the window
	static void reshape(GLFWwindow* window, int width, int height);
	static void processFrame(cv::Mat original, cv::Mat processed, KeyFinder keyFinder, Renderer& renderer, CMarkerFinder markerFinder);
	//void processFrameTest();
	
	GLFWwindow* window;
	CDur song;
	bool dead = false;

};

#endif //ARPEGGIO_RENDERER_H