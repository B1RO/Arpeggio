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


//Renderer class is responsible for preparing and generating the actual rendering 
class Renderer {

public:

	//constructor which enables the setup of the window, current song chosen by the user and mode, 
	Renderer(GLFWwindow* win, Music *song, bool mode):window(win){
		this->song = song;
		this->advanced = mode;
	}

	//initGL function adapted from tutorials, enables initialization and setup of openGL
	void initGL();

	//Reshaping function for the window
	static void reshape(GLFWwindow* window, int width, int height);

	//Functions that render in the current frame
	void renderAdvanced(vector<NoteToRender> notesToRender, Mat& frame);
	void renderBeginner(vector<NoteToRender> notesToRender, Mat& frame);

	/**
	* Function that prepares the rendering.
	* - accesses the 4 key contours per key 
	* - computes pose for each key  
	*/
	static void processFrame(cv::Mat original, cv::Mat processed, KeyFinder keyFinder, Renderer& renderer, CMarkerFinder markerFinder);
	
	GLFWwindow* window; //openGL window in which both the read frame and rendering is combined 
	Music *song; //current song chosen by the user 
	bool advanced = false; //defines whether the current mode is beginner or advanced

	bool dead = false; //indicates whether the window has been closed
};

#endif //ARPEGGIO_RENDERER_H