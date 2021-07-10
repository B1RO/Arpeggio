//
// Created by Miruna on 6/26/2021.
//
#ifndef ARPEGGIO_RENDERER_H
#define ARPEGGIO_RENDERER_H

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h> 
#include "../key-recognition/KeyFinder.h"
#include "../key-recognition/CMarkerFinder.h"
#include "../music/Pitch.h"
#include "../music/Music.h"
#include "../music/CBlues.h"
#include "../music/CDur.h"

	/**
	   * A note to render is a tuple of
	   * - its pose in opencv coordinates
	   * - a time in ms in which it will be played from wrapping class initialization,
	   * - a length of the note in ms,
	   * - an RGBA color that it shall be represented in.
	   */
typedef tuple<vector<float>, int, int, tuple<float, float, float, float>> NoteToRender;
class Renderer {


public:
	Renderer(GLFWwindow* win, Music *song, bool mode):window(win){
		this->song = song;
		this->mode = mode;
	}
	//OpenGL initialization function
	void initGL();

	//function that renders in the current frame
	void renderAdvanced(vector<NoteToRender> notesToRender, Mat& frame);
	void renderBeginner(vector<NoteToRender> notesToRender, Mat& frame);
	//reshaping function for the window
	static void reshape(GLFWwindow* window, int width, int height);
	static void processFrame(cv::Mat original, cv::Mat processed, KeyFinder keyFinder, Renderer& renderer, CMarkerFinder markerFinder);
	//void processFrameTest();
	
	GLFWwindow* window;
	Music *song;
	bool mode;
	bool dead = false;
	bool advanced = false;
};

#endif //ARPEGGIO_RENDERER_H