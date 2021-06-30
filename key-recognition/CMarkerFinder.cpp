//
// Created by marti on 6/26/2021.
//

#include "CMarkerFinder.h"
#include "ContourScaler.h"

void CMarkerFinder::processFrame(Mat frame) {
    this->sourceFrame = frame;
    this->findYellowMarkers(frame);
}

void CMarkerFinder::findYellowMarkers(Mat frame) {
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
    int smallestDiffIndex = 1;
    float smallestDiff = 10000;
    for (int i = 1; i < contours.size(); i++) {
        auto normalizedAreaPrev = contourArea(contours[i - 1]) / (frame.rows * frame.cols);
        auto normalizedArea = contourArea(contours[i]) / (frame.rows * frame.cols);
        //marker has to occupy at least 0.01% of screen
        if (normalizedAreaPrev > 0.001 && normalizedArea > 0.001) {
            float diff = abs(contourArea(contours[i]) - contourArea(contours[i - 1]));
            if (diff < smallestDiff) {
                smallestDiff = diff;
                smallestDiffIndex = i;
            }
        }
    }
    std::vector<contour_t> out;
    if (contours.size() > 1) {
        out.push_back(contours[smallestDiffIndex]);
        out.push_back(contours[smallestDiffIndex - 1]);
        this->yellowMarkers = out;
    }
    else
    {
        this->yellowMarkers = contours;
    }
}

vector<contour_t> CMarkerFinder::getYellowMarkers() {
    return this->yellowMarkers;
}

void CMarkerFinder::highlightMarkers(Mat frame) {
    auto scaled = ContourScaler::scaleContours(sourceFrame.cols, sourceFrame.rows, frame.cols, frame.rows, this->yellowMarkers);
    drawContours(frame, scaled, -1, Scalar(255, 0, 255), FILLED);
}