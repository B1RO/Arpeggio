//
// Created by Miruna on 6/26/2021.
//

#include <chrono>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "Renderer.h"
#include "DrawPrimitives.h"
#include "PoseEstimation.cpp"


//Camera settings for laptop camera
const int camera_width = 640;
const int camera_height = 480;

const int virtual_camera_angle = 41;
unsigned char bkgnd[camera_width * camera_height * 3];

//parameters needed for pose estimation
const double sizeWhiteTiles = 0.23;
const double sizeBlackTiles = 0.11;

struct Position { double x, y, z; };
Position ballPos;

//hardcoded parameters which enable visualization of the process
//inspite of the large distance between frames 
double ballSpeed = 0.0005;
double v0 = 0.0003;

//needed due to the looping manner of openGL to mark the beginning of an action 
bool firstTime = true; //marks the time when a note is reached by the ball
bool firstPress = true; //marks the first time a key is pressed
int nrNotes = 0;//initial number of notes need to determine when the transition from one note to the other happens
bool notInit = true; //the very first time the programm starts is marked
int maxTimeToHoldNote = 0; //the first time a note is pressed, the time to hold it is maximum, then it diminishes. 

//variables needed for the beginner mode, for the computation of the sphere position 
int timePrev;
int timeUntilNextNote;
double v_yi;

//helper variable for rendering the last single note in beginner mode (which normally needs a pair of notes)
bool sphere = false;


//Code adapted from the tutorials
void Renderer::initGL()
{
	// Set callback functions for GLFW
	glfwSetFramebufferSizeCallback(window, reshape);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	int window_width, window_height;
	glfwGetFramebufferSize(window, &window_width, &window_height);
	reshape(window,window_width, window_height);

	// For our connection between OpenCV/OpenGL
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelZoom(1.0, -1.0);

	glEnable(GL_COLOR_MATERIAL);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Enable and set depth parameters
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);

	// Light parameters
	GLfloat light_amb[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat light_pos[] = { 1.0, 1.0, 1.0, 0.0 };
	GLfloat light_dif[] = { 0.8, 0.8, 0.8, 1.0 };

	// Enable lighting
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_dif);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}
//--end Code adapted from the tutorials


//Function which maps the time which is still left to play from a certain key to the size of the cuboid
//only the height of the cuboid changes
float calculateSizeCuboid(int timeLeftToPlay)
{
	return float(timeLeftToPlay) / float(maxTimeToHoldNote) * 3 + 1;
}

//Function which renders in the advanced mode
//The functionality in the beginning is adapted from the tutorial code 
void Renderer::renderAdvanced(vector<NoteToRender> notesToRender, Mat& frame)
{
	//Code from the tutorials
	// Copy picture data into bkgnd array
	memcpy(bkgnd, frame.data, sizeof(bkgnd));

	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Needed for rendering the real camera image
	glMatrixMode(GL_MODELVIEW);

	//First render the camera image as background image 
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	// In the ortho view all objects stay the same size at every distance
	glOrtho(0.0, camera_width, 0.0, camera_height, -1, 1);
	glRasterPos2i(0, camera_height - 1);
	glDrawPixels(camera_width, camera_height, GL_BGR_EXT, GL_UNSIGNED_BYTE, bkgnd);

	// Go back to the previous projection and render the cubes 
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);

	//Transposing the ModelView matrix
	for (unsigned int i = 0; i < notesToRender.size(); ++i)
	{
		auto currNote = notesToRender[i];
		auto poseMatrix = std::get<0>(currNote);
		float resultTransposedMatrix[16];
		for (int x = 0; x < 4; ++x) {
			for (int y = 0; y < 4; ++y) {
				// Change columns to rows
				resultTransposedMatrix[x * 4 + y] = poseMatrix[y * 4 + x];
			}
		}
		glLoadMatrixf(resultTransposedMatrix);
		//end -- Code from the tutorials 

		auto color = std::get<3>(currNote);
		glColor4f(std::get<0>(color), std::get<1>(color), std::get<2>(color), std::get<3>(color));

		//needed for the last note of the beginner mode which needs to be played alone
		if (sphere)
			drawSphere(0.1, 10, 10);
		else
		{
			//the cuboid is first scaled and then rendered by computing it's height relative
			//to the time to play the current note
			glScalef(0.15, 0.15, 0.15);
			drawCuboid(calculateSizeCuboid(std::get<2>(currNote)), 2);
		}
	}
}

//Function which moves the sphere into the parabola trajectory
void moveSphere(NoteToRender nextNote)
{
	//obtain pose of next tile 
	auto nextNotePose = std::get<0>(nextNote);

	// distance vector between the current ball position and the next tile 
	float vector[3];
	vector[0] = nextNotePose[3] - ballPos.x;
	vector[1] = nextNotePose[7] - ballPos.y;
	vector[2] = nextNotePose[11] - ballPos.z;

	//length of the distance vector 
	float length = sqrt(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);

	//compute the time difference 
	auto deltaTime = timePrev - timeUntilNextNote;

	//hardcoded. Solves the problem of very high velocities in case the time until the next key is small
	if (timeUntilNextNote < 500)
		timeUntilNextNote = 500;
	
	//moving in a parabola
	{
		//compute the velocity of the ball in every iteration
		ballSpeed = length / timeUntilNextNote;
		
		//what needs to happen when a key is pressed the first time 
		if (firstPress)
		{
			v_yi = v0;
			firstPress = false;
			deltaTime = 0;
		}

		//update velocity of the ball in the perpendicular direction
		v_yi = v_yi + 2 * v0 * deltaTime / timeUntilNextNote;

		//find the vector perpendicular to the difference vector and the z vector 
		float zVector[3] = { 0.0,0.0,0.0 };
		//depends on whether the ball needs to jump from left to right or from right to left on the piano
		if (vector[0] > 0)
			zVector[2] = -1.0;
		else
			zVector[2] = 1.0;

		float dirBall[3];
		//cross product is the 3rd, perpendicular vector to the z direction and the distance vector between the ball position and the next tile
		dirBall[0] = vector[1] * zVector[2] - vector[2] * zVector[1];
		dirBall[1] = vector[2] * zVector[0] - vector[0] * zVector[2];
		dirBall[2] = vector[0] * zVector[1] - vector[1] * zVector[0];

		//update position based on the formula on the slides
		ballPos.x += (vector[0] / length) * ballSpeed * deltaTime + (deltaTime * v_yi) * dirBall[0];
		ballPos.y += (vector[1] / length) * (ballSpeed * deltaTime) + (deltaTime * v_yi) * dirBall[1];
		ballPos.z += (vector[2] / length) * ballSpeed * deltaTime + (deltaTime * v_yi) * dirBall[2];
	}
}

void Renderer::renderBeginner(vector<NoteToRender> notesToRender, Mat& frame)
{
	//only one note will be rendered at a time in beginner mode
	//notesToRender contains here 2 notes, the current and the next one
	if (notesToRender.size() > 2)
		return;

	//Code adapted from the tutorials
	// Copy picture data into bkgnd array
	memcpy(bkgnd, frame.data, sizeof(bkgnd));

	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Needed for rendering the real camera image
	glMatrixMode(GL_MODELVIEW);

	// No position changes
	//First render the camera image as background image 
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	// In the ortho view all objects stay the same size at every distance
	glOrtho(0.0, camera_width, 0.0, camera_height, -1, 1);
	glRasterPos2i(0, camera_height - 1);
	glDrawPixels(camera_width, camera_height, GL_BGR_EXT, GL_UNSIGNED_BYTE, bkgnd);

	// Go back to the previous projection and render the cubes 
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);

	//get the pose of the current note
	auto currNote = notesToRender[0];
	auto poseMatrixCurrNote = std::get<0>(currNote);

	//Transposing the ModelView matrix for next note
	auto nextNote = notesToRender[1];
	auto poseMatrixNextNote = std::get<0>(nextNote);
	float resultTransposedMatrixNextNote[16];
	for (int x = 0; x < 4; ++x) {
		for (int y = 0; y < 4; ++y) {
			// Change columns to rows
			resultTransposedMatrixNextNote[x * 4 + y] = poseMatrixNextNote[y * 4 + x];
		}
	}
	//-- end Code adapted from the tutorials

	//the very first time set the coordinates of the sphere to the coordinates of the first note
	if (firstTime)
	{
		ballPos.x = poseMatrixCurrNote[3];
		ballPos.y = poseMatrixCurrNote[7];
		ballPos.z = poseMatrixCurrNote[11];
		firstTime = false;
	}
	
	//update the time
	timePrev = timeUntilNextNote;
	auto timeLeftToPlay = std::get<2>(currNote);
	timeUntilNextNote = std::get<1>(nextNote);

	glLoadIdentity();

	//if less than half a second is left of the current note 
	//at the moment hardcoded due to frame resolution, otherwise the ball would not be visible
	//this represents the time between the prev tile is finished and the next tile needs to be played
	int timeBetweenNotes = 500;
	if (timeUntilNextNote < timeBetweenNotes)
		moveSphere(nextNote);
	else
	{
		//if the ball does not need to be transitioned, render it on the current note
		ballPos.x = poseMatrixCurrNote[3];
		ballPos.y = poseMatrixCurrNote[7];
		ballPos.z = poseMatrixCurrNote[11];
	}
		
	//go the ball position 
	glTranslatef(ballPos.x, ballPos.y + 0.024, ballPos.z);
	//get the color of the current note
	auto color = std::get<3>(currNote);
	glColor4f(std::get<0>(color), std::get<1>(color), std::get<2>(color), std::get<3>(color));
	//draw the sphere
	drawSphere(0.1, 10, 10);

}

//Reshaping code from the tutorial
void Renderer::reshape(GLFWwindow *window, int width, int height) {
    // Set a whole-window viewport
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);

    // Create a perspective projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float ratio = (GLfloat) width / (GLfloat) height;
    float near = 0.01f, far = 100.f;
    float top = tan((double) (virtual_camera_angle * M_PI / 360.0f)) * near;
    float bottom = -top;
    float left = ratio * bottom;
    float right = ratio * top;
    glFrustum(left, right, bottom, top, near, far);
}
//--end reshaping code from the tutorial

//The function generates a tuple representing a note which contains all of the info for rendering
NoteToRender createNoteToRender(Note noteToProcess, Mat& original, KeyFinder keyFinder)
{
	//get the pitch of the current note
	auto currPitch = std::get<0>(noteToProcess);
	//get the contour from the keyfinder that corresponds to the pitch
	auto contour = keyFinder.getKeyContour(currPitch);
	vector<float> resMat;

	//if the contour was found
	if (contour)
	{
		//get the corners of the current tile needed for the pose estimation
		contour_t corners = keyFinder.molestPianoKeyIntoASquare(*contour);
		cv::Point2f cornersPose[4];

		cornersPose[0] = corners[0];
		cornersPose[1] = corners[1];
		cornersPose[2] = corners[2];
		cornersPose[3] = corners[3];

		//transform into camera coordinates
		for (int i = 0; i < 4; i++)
		{
			cornersPose[i].x -= original.cols * 0.5;
			cornersPose[i].y = -cornersPose[i].y + original.rows * 0.5;
		}

		//pose estimation
		float resultMatrix[16];

		//needed 2 different types of pose estimation, because white and black keys have different sizes
		if (isBlackKey(currPitch))
			estimateSquarePose(resultMatrix, (cv::Point2f*)cornersPose, sizeBlackTiles);
		else
			estimateSquarePose(resultMatrix, (cv::Point2f*)cornersPose, sizeWhiteTiles);
		for (int j = 0; j < 16; ++j)
			resMat.push_back(resultMatrix[j]);

		//get information needed for the tuple
		auto whenToPlay = std::get<1>(noteToProcess);
		auto howLongToPlay = std::get<2>(noteToProcess);
		auto color = std::get<3>(noteToProcess);

		//create and return tuple 
		return tuple(resMat, whenToPlay, howLongToPlay, color);

	}
	//in case the contour could not be found, signalise
	return tuple(resMat, -5, 0, std::get<3>(noteToProcess));


}

//In the beginning of the programm, the longest time a note will be held is computed
//This aids in the calculation of the linear mapping of the size of the cuboid
void computeMaxTimeToPlay(vector<Note> currNotes)
{
	for (Note currNote : currNotes) {
		auto timeLeftToPlayCurrNote = std::get<2>(currNote);
		if (timeLeftToPlayCurrNote > maxTimeToHoldNote)
			maxTimeToHoldNote = timeLeftToPlayCurrNote;
	}
}

//This function prepares the rendering and decides upon the modus
//It obtains the notes that will be played and transforms them into a format needed by the renderer
void Renderer::processFrame(cv::Mat original, cv::Mat processed, KeyFinder keyFinder, Renderer& renderer, CMarkerFinder markerFinder)
{

	//verify whether the user has closed the window and turn the flag to true
	if (glfwWindowShouldClose(renderer.window))
	{
		renderer.dead = true;
		return;
	}
	
	//get the notes from the current song 
	vector<Note> currNotes = renderer.song->notes();
	vector<NoteToRender> notesToRender;

	if (renderer.advanced)
	{
		//in the beginning of the programm, compute the longest time a key will be played
		if (notInit)
		{
			computeMaxTimeToPlay(currNotes);
			notInit = false;
		}
		//advanced mode --> play only current note
		//iterate over the notes and transform them into the adequate format for rendering
		for (Note currNote : currNotes) {
			if (std::get<1>(currNote) == 0)
			{
				NoteToRender noteToRender = createNoteToRender(currNote, original, keyFinder);
				//verify whether the contour was found
				if (std::get<1>(noteToRender) != -5)
					notesToRender.push_back(noteToRender);
			}
				
		}
		//finally call the rendering function
		renderer.renderAdvanced(notesToRender, original);

	} //end advanced mode
	else
	{	
		//compute how many notes still will be played
		auto nrCurrentNotes = currNotes.size();

		//beginner mode --> process the current and the next tile at a time 
		if (nrCurrentNotes < 2)
		{
			//in case only one note is left, bypass and jump to advance mode but with a sphere
			renderer.advanced = true;
			sphere = true;
			return;
		}
		
		//the first time save the number of notes 
		if (firstTime)
			nrNotes = nrCurrentNotes;

		//determine whether it is the first time a note was pressed
		if (nrCurrentNotes < nrNotes)
		{
			nrNotes = nrCurrentNotes;
			firstPress = true;
		}
		
		//assume that it s always the first and the second note that needs to be played 
		Note pair[2] = { currNotes[0], currNotes[1] };
		for (int i = 0; i < 2; ++i)
			notesToRender.push_back(createNoteToRender(pair[i], original, keyFinder));
		
		//finally call the renderer function
		renderer.renderBeginner(notesToRender, original);

	}//end beginner mode


	//Code from the tutorials
	// 
	// Swap front and back buffers
	glfwSwapBuffers(renderer.window);
	// Poll for and process events
	glfwPollEvents();
	
	//--end Code from the tutorials
}

