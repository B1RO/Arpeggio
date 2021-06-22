//
// Created by marti on 6/22/2021.
//

#include "KeyFinder.h"

Point getContourCentroid(contour_t contour) {
    auto M = moments(contour);
    auto cX = int(M.m10 / M.m00);
    auto cY = int(M.m01 / M.m00);
    return Point(cX, cY);
}

void KeyFinder::processFrame(Mat frame) {
    contour_vector_t allContours;
    findContours(dilateKeys(thresholdKeys(frame)), allContours, RETR_LIST, CHAIN_APPROX_SIMPLE);

    contour_vector_t filteredContours;
    for (int i = 0; i < allContours.size(); i++) {
        filteredContours.push_back(allContours[i]);
    }

    //sort contours by centroid x position
    sort(filteredContours.begin(), filteredContours.end(),
         [](const contour_t &a, const contour_t &b) -> bool {
             return getContourCentroid(a).x < getContourCentroid(b).x;
         });

    this->keyContours = filteredContours;
}

const int max_value_H = 360 / 2;
const int max_value = 255;

Mat KeyFinder::thresholdKeys(Mat frame) {
    Mat frame_HSV;
    Mat frame_threshold;
    cvtColor(frame, frame_HSV, COLOR_BGR2HSV);
    int low_H = 0, low_S = 0, low_V = 175;
    int high_H = max_value_H, high_S = max_value, high_V = max_value;
    inRange(frame_HSV, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), frame_threshold);
    return frame_threshold;
}

Mat KeyFinder::dilateKeys(Mat frame) {
    int dilation_size = 3;
    Mat element = getStructuringElement(MORPH_RECT,
                                        Size(2 * dilation_size + 1, 2 * dilation_size + 1),
                                        Point(dilation_size, dilation_size));
    Mat eroded;
    erode(frame, eroded, element);
    return eroded;
}

void KeyFinder::colorPianoKeysIntoFrame(Mat frame) {
    for (int i = 0; i < this->keyContours.size(); i++) {
        drawContours(frame, this->keyContours, i, Scalar(rand() & 256, rand() % 256, rand() % 256), FILLED);
    }

}

void KeyFinder::specifyCs(vector<Point> CKeys) {
    this->CKeys = std::move(CKeys);
}

void KeyFinder::__debug__renderCCentroids(Mat frame) {
    for (const auto &item : this->CKeys) {
        circle(frame,
               item,
               5,
               Scalar(0, 0, 255),
               FILLED,
               LINE_8);
    }
}

int convertPianoKeyToOffsetFromC(char key) {
    switch (key) {
        case 'C':
            return 0;
        case 'D':
            return 1;
        case 'E':
            return 2;
        case 'F':
            return 3;
        case 'G':
            return 4;
        case 'A':
            return 5;
        case 'B':
            return 6;
    }
}

void KeyFinder::colorKey(Mat frame, char key, Scalar color) {
    vector<Scalar> keyColors = {{183, 28, 28},
                                {136, 14, 79},
                                {74,  20, 140},
                                {26,  35, 126},
                                {1,   87, 155},
                                {0,   77, 64}};
    int offset = convertPianoKeyToOffsetFromC(key);
    for (const auto &item : this->CKeys) {
        auto contourIndex = this->getIndexOfContourClosestToPoint(item);
        drawContours(frame, vector<vector<Point> >(1, this->keyContours[contourIndex + offset]), 0, keyColors[offset],
                     FILLED);
    }
}

int KeyFinder::getIndexOfContourClosestToPoint(Point point) {
    float minDist = 100000;
    int minIndex = 0;
    for (int i = 0; i < this->keyContours.size(); i++) {

        auto centroid = getContourCentroid(this->keyContours[i]);
        auto dist = cv::norm(centroid - point);
        if (dist < minDist) {
            minIndex = i;
            minDist = dist;
        }
    }
    return minIndex;
}

void KeyFinder::labelKey(Mat frame, char key) {
    int offset = convertPianoKeyToOffsetFromC(key);
    for (const auto &item : this->CKeys) {
        auto contourIndex = this->getIndexOfContourClosestToPoint(item);
        auto offset = convertPianoKeyToOffsetFromC(key);
        auto contour = this->keyContours[contourIndex + offset];
        auto centroid = getContourCentroid(contour);
        putText(frame, std::string(1, key), centroid - Point(5, -5), FONT_HERSHEY_DUPLEX, 0.5, CV_RGB(255, 0, 0), 1);
    }
}
