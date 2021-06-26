//
// Created by marti on 6/25/2021.
//




#ifndef ARPEGGIO_VIDEOFEEDTRANSFORMER_H
#define ARPEGGIO_VIDEOFEEDTRANSFORMER_H
#include <opencv2/opencv.hpp>
#include "KeyFinder.h"
#include "CMarkerFinder.h"

using namespace cv;

class VideoFeedTransformer {
public:
    void consumeFeed(VideoCapture capture, std::function<void(Mat,Mat,KeyFinder,CMarkerFinder)> onTransformed);
private:
    Mat processFrame(Mat source);
    KeyFinder keyFinder;
    CMarkerFinder markerFinder;
};


#endif //ARPEGGIO_VIDEOFEEDTRANSFORMER_H
