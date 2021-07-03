//
// Created by marti on 6/25/2021.
//

#include "VideoFeedTransformer.h"
#include <iostream>
#include<opencv2/opencv.hpp>
#include <numeric>


using namespace cv;
using namespace std;

//Mat frame, bw, dst, detected_edges, markers;

typedef vector<Point> contour_t;
typedef vector<contour_t> contour_vector_t;

const char *original = "Original";
const char *processed = "Processed";
const char *yellow = "Yellow";

int lowThreshold = 20;
const int max_lowThreshold = 100;
const int t_ratio = 3;
const int kernel_size = 3;
int erosion_size = 2;

vector<contour_t> findYellowMarkers(Mat frame) {
    Mat imgHSV;
    cvtColor(frame, imgHSV, COLOR_BGR2HSV);
    inRange(imgHSV, Scalar(20, 100, 100), Scalar(40, 255, 255), imgHSV);
    contour_vector_t contours;
    findContours(imgHSV, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
    //sort markers by area
    sort(contours.begin(), contours.end(),
         [](const contour_t &a, const contour_t &b) -> bool {
             return contourArea(a) > contourArea(b);
         });
    //find the two contours with the smallest difference between areas (since the markers are the same size)
    int smallestDiffIndex = 0;
    float smallestDiff = 10000;
    for (int i = 1; i < contours.size(); i++) {
        if (contourArea(contours[i]) > 20 && contourArea(contours[i - 1]) > 20) {
            float diff = abs(contourArea(contours[i]) - contourArea(contours[i - 1]));
            if (diff < smallestDiff) {
                smallestDiff = diff;
                smallestDiffIndex = i;
            }
        }
    }
    std::vector<contour_t> out;
    out.push_back(contours[smallestDiffIndex]);
    //out.push_back(contours[smallestDiffIndex - 1]);
    return out;
}

contour_t findLeftYellowMarker(contour_vector_t yellowMarkers) {
    int index = 0;
    for (int i = 0; i < yellowMarkers.size(); i++) {
        if (yellowMarkers[i][0].x < yellowMarkers[index][0].x) {
            index = i;
        }
    }
    return yellowMarkers[index];
}

Mat downscaleFrame(Mat frame) {
    //Size size1(384, 216);
    Size size1(640, 480);
    resize(frame, frame, size1);
    return frame;
}

Mat preprocessFrame(Mat frame) {
    Mat bw;
    cvtColor(frame, bw, CV_BGR2GRAY);
    threshold(bw, bw, 150, 255,
              THRESH_BINARY); // 75 for somewhat combined white area, 150 for more individual keys
//    Mat element = getStructuringElement(MORPH_RECT,
//                                        Size(2 * erosion_size + 1, 2 * erosion_size + 1),
//                                        Point(erosion_size, erosion_size));
//    morphologyEx(bw, bw, 2, element);
//    morphologyEx(bw, bw, 3, element);
    return bw;
}


vector<double> getAreas(contour_vector_t contours) {
    vector<double> areas(contours.size());
    for (int i = 0; i < contours.size(); i++) {
        areas.push_back(contourArea(contours[i]));
    }
    return areas;
}

double getAverageArea(vector<double> areas) {
    return accumulate(areas.begin(), areas.end(), 0.0) / areas.size();
}

double getAreaStdDev(vector<double> areas) {
    double mean = getAverageArea(areas);
    double sq_sum = inner_product(areas.begin(), areas.end(), areas.begin(), 0.0);
    double stdev = sqrt(sq_sum / areas.size() - mean * mean);
    return stdev;
}

contour_vector_t getContoursWithAreaCloseWithinMean(contour_vector_t in) {
    vector<double> areas = getAreas(in);
    for (int i = 0; i < areas.size(); i++) {
        cout << areas[i] << endl;
    }
    double average = getAverageArea(areas);
    cout << "Average: " << average << endl << endl << endl;
    double stddev = getAreaStdDev(areas);
    contour_vector_t out;
    for (const auto &item : in) {
        if (abs(contourArea(item) - average) < stddev / 2) {
            out.push_back(item);
        }
    }
    return out;
}


vector<Point> getCentroidsFromContours(contour_vector_t contours) {
    vector<Point> centroids;
    for (const auto &item : contours) {
        auto M = moments(item);
        auto cX = int(M.m10 / M.m00);
        auto cY = int(M.m01 / M.m00);
        centroids.push_back(Point(cX, cY));
    }
    return centroids;
}

Mat thresholdYellowObjects(Mat frame) {
    Mat frame_HSV;
    Mat frame_threshold;
    cvtColor(frame, frame_HSV, COLOR_BGR2HSV);
    inRange(frame_HSV, Scalar(20, 100, 100), Scalar(30, 255, 255), frame_threshold);
    return frame_threshold;
}


void VideoFeedTransformer::consumeFeed(VideoCapture capture, std::function<void(Mat, Mat, KeyFinder,  Renderer& renderer, CMarkerFinder)> onTransformed,  Renderer& renderer ) {
    Mat frame;
    while (capture.read(frame)) {
        auto downscaled = downscaleFrame(frame);
        Mat processed = processFrame(downscaled);
        onTransformed(downscaled, processed, keyFinder, renderer, markerFinder);
        if (renderer.dead)
            break;
        int key = waitKey(60);
       /* if (key == 27)
            break;*/

    }
 
}

Mat VideoFeedTransformer::processFrame(Mat frame) {
    auto processed = keyFinder.processFrame(frame);
    markerFinder.processFrame(frame);
    keyFinder.specifyCs(getCentroidsFromContours(markerFinder.getYellowMarkers()));
    return processed;
}
