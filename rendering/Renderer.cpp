//
// Created by Miruna on 6/26/2021.
//

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

void Renderer::initGL() {
    // Set callback functions for GLFW
    glfwSetFramebufferSizeCallback(window, reshape);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    int window_width, window_height;
    glfwGetFramebufferSize(window, &window_width, &window_height);
    reshape(window, window_width, window_height);

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
    GLfloat light_amb[] = {0.2, 0.2, 0.2, 1.0};
    GLfloat light_pos[] = {1.0, 1.0, 1.0, 0.0};
    GLfloat light_dif[] = {0.7, 0.7, 0.7, 1.0};

    // Enable lighting
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_dif);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void Renderer::render(vector<NoteToRender> notesToRender, Mat &frame) {
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

    for (unsigned int i = 0; i < notesToRender.size(); ++i) {
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

        // Just because
        glRotatef(90, 1, 0, 0);

        // Scale, otherwise monster spheres
        glScalef(0.2, 0.2, 0.2);

        auto color = std::get<1>(currNote);
        glColor4f(std::get<0>(color), std::get<1>(color), std::get<2>(color), std::get<3>(color));
        drawSphere(0.6, 10, 10);


    }
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

void Renderer::processFrame(cv::Mat original, cv::Mat processed, KeyFinder keyFinder, Renderer &renderer,
                            CMarkerFinder markerFinder) {
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
    if (glfwWindowShouldClose(renderer.window)) {
        renderer.dead = true;
        return;
    }

    vector<Note> currNotes = renderer.song->notes();
    vector<NoteToRender> notesToRender;
    //vector <Pitch> testPitches;
    //testPitches.push_back(Pitch::C4);
    //testPitches.push_back(Pitch::D4);
    //testPitches.push_back(Pitch::E4);

    for (Note currNote : currNotes) {
        if (std::get<1>(currNote) == 0) {
            auto currPitch = std::get<0>(currNote);
            //for (Pitch currPitch : testPitches)
            //{
            auto contour = keyFinder.getKeyContour(currPitch);
            if (contour.has_value() && contour.value().size() > 0) {
                contour_t corners = KeyFinder::molestPianoKeyIntoASquare(contour.value());
                cv::Point2f cornersPose[4];

                cornersPose[0] = corners[3];
                cornersPose[1] = corners[2];
                cornersPose[2] = corners[1];
                cornersPose[3] = corners[0];
                //cv::circle(original, cornersPose[0], 5, CV_RGB(255, 0, 0), -1);
                //cv::circle(original, cornersPose[1], 5, CV_RGB(0, 255, 0), -1);
                //cv::circle(original, cornersPose[2], 5, CV_RGB(0, 0, 255), -1);
                //cv::circle(original, cornersPose[3], 5, CV_RGB(0, 0, 0), -1);
                // transfer screen coords to camera coords
                for (int i = 0; i < 4; i++) {
                    cornersPose[i].x -= original.cols * 0.5;
                    cornersPose[i].y = -corners[i].y + original.rows * 0.5;
                }

                //imshow("window", processed);
                //imshow("original", original);
                //find its pose
                float resultMatrix[16];
                if (isBlackKey(currPitch))
                    estimateSquarePose(resultMatrix, (cv::Point2f *) cornersPose, sizeBlackTiles);
                else
                    estimateSquarePose(resultMatrix, (cv::Point2f *) cornersPose, sizeWhiteTiles);
                vector<float> resMat;
                for (int j = 0; j < 16; ++j)
                    resMat.push_back(resultMatrix[j]);
                notesToRender.emplace_back(resMat, std::get<3>(currNote));
            }
        }
    }

    renderer.render(notesToRender, original);
    // Swap front and back buffers
    glfwSwapBuffers(renderer.window);
    // Poll for and process events
    glfwPollEvents();
}

