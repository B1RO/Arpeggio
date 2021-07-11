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


// Camera settings for laptop camera
//Now also for the rest due to scaling
const int camera_width = 640;
const int camera_height = 480;

//Camera settings for video sample_video_hands
//const int camera_width = 1920;
//const int camera_height = 1080;

//Camera settings for video samplevideo1
//const int camera_width = 3840;
//const int camera_height = 2160;

const int virtual_camera_angle = 41;
unsigned char bkgnd[camera_width * camera_height * 3];

//needed for pose estimation
const double sizeWhiteTiles = 0.23;
const double sizeBlackTiles = 0.11;

struct Position { double x, y, z; };
Position ballPos;


double ballSpeed = 0.0005;
double v0 = 0.0003;
bool firstTime = true;
auto tPrev = 0;
bool firstPress = true;

int nrNotes = 0;
bool notInit = true;

int timePrev;
int timeUntilNextNote;
double v_yi;

int maxTimeToHoldNote = 0;

bool sphere = false;

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
	GLfloat light_amb[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light_pos[] = { 1.0, 1.0, 1.0, 0.0 };
	GLfloat light_dif[] = { 0.7, 0.7, 0.7, 1.0 };

	// Enable lighting
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_dif);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

float calculateSizeCuboid(int timeLeftToPlay)
{
	return float(timeLeftToPlay) / float(maxTimeToHoldNote) * 6;
}

void Renderer::renderAdvanced(vector<NoteToRender> notesToRender, Mat& frame)
{
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
		//resultTransposedMatrix[1] -= 0.5;
		glLoadMatrixf(resultTransposedMatrix);

		auto color = std::get<3>(currNote);
		glColor4f(std::get<0>(color), std::get<1>(color), std::get<2>(color), std::get<3>(color));

		if (sphere)
			drawSphere(0.1, 10, 10);
		else
		{
			glScalef(0.15, 0.15, 0.15);

			//TODO, decide here which strategy to take
			//drawCuboid(std::get<2>(currNote) / 200, std::get<2>(currNote) / 200);
			//drawCuboid(std::get<2>(currNote) / 200, 2);

			//drawCuboid(calculateSizeCuboid(std::get<2>(currNote)), calculateSizeCuboid(std::get<2>(currNote)));
			drawCuboid(calculateSizeCuboid(std::get<2>(currNote)), 2);
			//drawCuboid(2.9, 2.9);
		}
		
	

	}
}

void moveSphere(NoteToRender nextNote)
{
	auto nextNotePose = std::get<0>(nextNote);
	// Where is the ball correponding to the next tile 
	float vector[3];
	vector[0] = nextNotePose[3] - ballPos.x;
	vector[1] = nextNotePose[7] - ballPos.y;
	vector[2] = nextNotePose[11] - ballPos.z;

	float length = sqrt(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);

	auto deltaTime = timePrev - timeUntilNextNote;

	//auto deltaTime = 0;
	//cout << "delta time:" << deltaTime << endl;
	//{
	////so that it does not overshoot in the last time before arrival
	//	if (timeUntilNextNote < 50)
	//		timeUntilNextNote = 500;
	//	//moving in a straight line
	//	ballSpeed = length / timeUntilNextNote;
	//	cout << "timeUntilNextNote:" << timeUntilNextNote << endl;
	//	ballPos.x += (vector[0] / length) * ballSpeed * deltaTime;
	//	ballPos.y += (vector[1] / length) * ballSpeed * deltaTime;
	//	ballPos.z += (vector[2] / length) * ballSpeed * deltaTime;
	//}

	if (timeUntilNextNote < 100)
		timeUntilNextNote = 500;
	
	//moving in a (kind of)parabola
	{
		ballSpeed = length / timeUntilNextNote;
		
		if (firstPress)
		{
			v_yi = v0;
			firstPress = false;
			deltaTime = 0;
			//cout << "firstPress;";
		}

		v_yi = v_yi + 2 * v0 * deltaTime / timeUntilNextNote;

		//find the vector perpendicular to the difference vector and the z vector 
		float zVector[3] = { 0.0,0.0,0.0 };
		if (vector[0] > 0)
			zVector[2] = -1.0;
		else
			zVector[2] = 1.0;

		//a = vector , b = zVector 
		float dirBall[3];
		//cross product is the 3rd, perpendicular vector 
		dirBall[0] = vector[1] * zVector[2] - vector[2] * zVector[1];
		dirBall[1] = vector[2] * zVector[0] - vector[0] * zVector[2];
		dirBall[2] = vector[0] * zVector[1] - vector[1] * zVector[0];

		ballPos.x += (vector[0] / length) * ballSpeed * deltaTime + (deltaTime * v_yi) * dirBall[0];
		ballPos.y += (vector[1] / length) * (ballSpeed * deltaTime) + (deltaTime * v_yi) * dirBall[1];
		ballPos.z += (vector[2] / length) * ballSpeed * deltaTime + (deltaTime * v_yi) * dirBall[2];
	}

	
	//cout << ";ballPos.x:" << ballPos.x <<  ";ballPos.y:" << ballPos.y << ";ballPos.z:" << ballPos.z;
	//cout << ";timeUntilNextNote: " << timeUntilNextNote <<end;
}

void Renderer::renderBeginner(vector<NoteToRender> notesToRender, Mat& frame)
{
	//only one note will be rendered at a time in beginner mode
	//notesToRender contains here 2 notes, the current and the next one
	if (notesToRender.size() > 2)
		return;
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

	//the very first time set the coordinates of the sphere to the coordinates of the first note
	if (firstTime)
	{
		ballPos.x = poseMatrixCurrNote[3];
		ballPos.y = poseMatrixCurrNote[7];
		ballPos.z = poseMatrixCurrNote[11];
		firstTime = false;
	}
	
	timePrev = timeUntilNextNote;
	auto timeLeftToPlay = std::get<2>(currNote);
	timeUntilNextNote = std::get<1>(nextNote);

	glLoadIdentity();

	//if less than half a second is left of the current note 
	if (timeUntilNextNote < 500)
		moveSphere(nextNote);

	else
	{
		ballPos.x = poseMatrixCurrNote[3];
		ballPos.y = poseMatrixCurrNote[7];
		ballPos.z = poseMatrixCurrNote[11];
	}
		
	
	glTranslatef(ballPos.x, ballPos.y + 0.024, ballPos.z);
	auto color = std::get<3>(currNote);
	glColor4f(std::get<0>(color), std::get<1>(color), std::get<2>(color), std::get<3>(color));
	drawSphere(0.1, 10, 10);

}

void Renderer::reshape(GLFWwindow *window, int width, int height) {
    // Set a whole-window viewport
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);

    // Create a perspective projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // The camera should be calibrated -> a calibration results in the projection matrix -> then load the matrix
    // -> into GL_PROJECTION
    // -> adjustment of FOV is needed for each camera
    float ratio = (GLfloat) width / (GLfloat) height;
    float near = 0.01f, far = 100.f;
    float top = tan((double) (virtual_camera_angle * M_PI / 360.0f)) * near;
    float bottom = -top;
    float left = ratio * bottom;
    float right = ratio * top;
    glFrustum(left, right, bottom, top, near, far);
}

NoteToRender createNoteToRender(Note noteToProcess, Mat& original, KeyFinder keyFinder)
{
	auto currPitch = std::get<0>(noteToProcess);
	auto contour = keyFinder.getKeyContour(currPitch);
	vector<float> resMat;

	if (contour)
	{
		contour_t corners = keyFinder.molestPianoKeyIntoASquare(*contour);
		cv::Point2f cornersPose[4];

		cornersPose[0] = corners[0];
		cornersPose[1] = corners[1];
		cornersPose[2] = corners[2];
		cornersPose[3] = corners[3];
		//cv::circle(original, cornersPose[0], 5, CV_RGB(255, 0, 0), -1);
		//cv::circle(original, cornersPose[1], 5, CV_RGB(0, 255, 0), -1);
		//cv::circle(original, cornersPose[2], 5, CV_RGB(0, 0, 255), -1);
		//cv::circle(original, cornersPose[3], 5, CV_RGB(0, 0, 0), -1);
		// transfer screen coords to camera coords
		for (int i = 0; i < 4; i++)
		{
			cornersPose[i].x -= original.cols * 0.5;
			cornersPose[i].y = -cornersPose[i].y + original.rows * 0.5;
		}

		//imshow("window", processed);
		//imshow("original", original);
		//find its pose 
		float resultMatrix[16];
		if (isBlackKey(currPitch))
			estimateSquarePose(resultMatrix, (cv::Point2f*)cornersPose, sizeBlackTiles);
		else
			estimateSquarePose(resultMatrix, (cv::Point2f*)cornersPose, sizeWhiteTiles);
		for (int j = 0; j < 16; ++j)
			resMat.push_back(resultMatrix[j]);

		auto whenToPlay = std::get<1>(noteToProcess);
		auto howLongToPlay = std::get<2>(noteToProcess);
		auto color = std::get<3>(noteToProcess);
		return tuple(resMat, whenToPlay, howLongToPlay, color);

	}
	return tuple(resMat, -5, 0, std::get<3>(noteToProcess));


}

void computeMaxTimeToPlay(vector<Note> currNotes)
{
	for (Note currNote : currNotes) {
		auto timeLeftToPlayCurrNote = std::get<2>(currNote);
		if (timeLeftToPlayCurrNote > maxTimeToHoldNote)
			maxTimeToHoldNote = timeLeftToPlayCurrNote;
	}
}
void Renderer::processFrame(cv::Mat original, cv::Mat processed, KeyFinder keyFinder, Renderer& renderer, CMarkerFinder markerFinder)
{
	//keyFinder.colorPianoKeysIntoFrame(processed);
	//keyFinder.colorKey(processed, Pitch::C1, Scalar(255, 0, 255));
	//keyFinder.labelKey(processed, Pitch::C1);
	//keyFinder.colorKey(processed, Pitch::A1, Scalar(255, 0, 255));
	//keyFinder.labelKey(processed, Pitch::A1);
	//
	//keyFinder.__debug__renderPianoSquares(original);
	//imshow("window", processed);
	//imshow("original", original);

	//trick opengl into thinking he is the most outer loop of the program
	if (glfwWindowShouldClose(renderer.window))
	{
		renderer.dead = true;
		return;
	}
		
	vector<Note> currNotes = renderer.song->notes();
	vector<NoteToRender> notesToRender;

	if (renderer.advanced)
	{
		if (notInit)
		{
			computeMaxTimeToPlay(currNotes);
			notInit = false;
		}
		//advanced mode --> play only current note
		for (Note currNote : currNotes) {
			if (std::get<1>(currNote) == 0)
			{
				NoteToRender noteToRender = createNoteToRender(currNote, original, keyFinder);
				if (std::get<1>(noteToRender) != -5)
					notesToRender.push_back(noteToRender);
			}
				
		}
		renderer.renderAdvanced(notesToRender, original);

	} //end advanced mode
	else
	{	
		auto nrCurrentNotes = currNotes.size();

		//beginner mode --> process the current and the next tile at a time 
		if (nrCurrentNotes < 2)
		{
			renderer.advanced = true;
			sphere = true;
			return;
		}
		
		//the first time save the number of notes 
		if (firstTime)
			nrNotes = nrCurrentNotes;

		if (nrCurrentNotes < nrNotes)
		{
			nrNotes = nrCurrentNotes;
			firstPress = true;
		}
		
		//assume that it s always the first and the second note that needs to be played 
		Note pair[2] = { currNotes[0], currNotes[1] };
		for (int i = 0; i < 2; ++i)
			notesToRender.push_back(createNoteToRender(pair[i], original, keyFinder));
		
		renderer.renderBeginner(notesToRender, original);

	}//end beginner mode

	// Swap front and back buffers
	glfwSwapBuffers(renderer.window);
	// Poll for and process events
	glfwPollEvents();
}

