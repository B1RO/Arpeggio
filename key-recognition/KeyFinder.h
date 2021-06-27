//
// Created by marti on 6/22/2021.
//
#ifndef ARPEGGIO_KEYFINDER_H
#define ARPEGGIO_KEYFINDER_H

#include<opencv2/opencv.hpp>
#include "./../music/Pitch.h"

using namespace cv;
using namespace std;


typedef vector<Point> contour_t;
typedef vector<contour_t> contour_vector_t;

class KeyFinder {
public:
    Mat processFrame(Mat frame);

    void colorPianoKeysIntoFrame(Mat frame);

    void __debug__renderCCentroids(Mat frame);
    void __debug__renderPianoSquares(Mat frame);
    void __debug__renderPianoRectangles(Mat frame);
    void specifyCs(vector<Point> CKeys);
    void colorKey(Mat frame, Pitch pitch, Scalar color);
    void labelKey(Mat frame, Pitch pitch);
    contour_t getKeyContour(Pitch pitch);
    Point getKeyCentroid(Pitch pitch);
    static contour_t molestPianoKeyIntoASquare(contour_t contour);
    static contour_t molestPianoKeyIntoARectangle(contour_t contour);
    vector<contour_t> getKeysAsSquares();
    vector<contour_t> getKeysAsRectangles();
private:
    int getIndexOfContourClosestToPoint(Point point);
    std::vector<Point> yellowMarkers;
    std::vector<contour_t> keyContours;
    static Mat thresholdKeys(Mat frame);
    static Mat thresholdKeysGrayscale(Mat frame);
    static Mat dilateKeys(Mat frame);
};


#endif //ARPEGGIO_KEYFINDER_H
