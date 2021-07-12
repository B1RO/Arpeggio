//
// Created by marti on 6/22/2021.
//
#ifndef ARPEGGIO_KEYFINDER_H
#define ARPEGGIO_KEYFINDER_H

#include<opencv2/opencv.hpp>
#include "./../music/Pitch.h"
#include <optional>

using namespace cv;
using namespace std;

typedef vector<Point> contour_t;
typedef vector<contour_t> contour_vector_t;

class KeyFinder {
public:
    Mat processFrame(Mat frame);
    Mat processFrameBlackKeys(Mat frame);
    void colorPianoKeysIntoFrame(Mat frame);
    void colorBlackPianoKeysIntoFrame(Mat frame);
    void __debug__renderCCentroids(Mat frame);
    void __debug__renderPianoSquares(Mat frame);
    void __debug__renderPianoRectangles(Mat frame);
    void specifyCs(vector<Point> CKeys);
    void colorKey(Mat frame, Pitch pitch, Scalar color);
    void labelKey(Mat frame, Pitch pitch);
    optional<Point> getKeyCentroid(Pitch pitch);
    optional<contour_t> getKeyContour(Pitch pitch);
    static contour_t molestPianoKeyIntoASquare(contour_t contour);
    static contour_t molestPianoKeyIntoARectangle(contour_t contour);
    vector<contour_t> getKeysAsSquares();
    vector<contour_t> getKeysAsRectangles();
private:
    Mat sourceFrame;
    int getIndexOfContourClosestToPoint(Point point, contour_vector_t contours);
    std::vector<Point> yellowMarkers;
    std::vector<contour_t> keyContours;
    //I don't own a piano please don't judge me I don't know how they are called.I am sure the left leftmost black key is called Josh though.
    std::vector<contour_t> blackKeyContours;
    static Mat thresholdKeys(Mat frame);
    static Mat thresholdKeysGrayscale(Mat frame);
    static Mat thresholdKeysBlack(const Mat& frame);
    static Mat dilateKeys(Mat frame);

    static Mat getPianoMask(const Mat &frame);
};


#endif //ARPEGGIO_KEYFINDER_H
