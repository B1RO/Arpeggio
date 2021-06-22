//
// Created by marti on 6/22/2021.
//
#ifndef ARPEGGIO_KEYFINDER_H
#define ARPEGGIO_KEYFINDER_H

#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;


typedef vector<Point> contour_t;
typedef vector<contour_t> contour_vector_t;

class KeyFinder {
public:
    void processFrame(Mat frame);

    void colorPianoKeysIntoFrame(Mat frame);

    void __debug__renderCCentroids(Mat frame);

    void specifyCs(vector<Point> CKeys);
    void colorKey(Mat frame, char key, Scalar color);
    void labelKey(Mat frame, char key);
private:
    int getIndexOfContourClosestToPoint(Point point);
    std::vector<Point> CKeys;
    std::vector<contour_t> keyContours;
    static Mat thresholdKeys(Mat frame);
    static Mat dilateKeys(Mat frame);
};


#endif //ARPEGGIO_KEYFINDER_H
