//
// Created by marti on 6/25/2021.
//




#ifndef ARPEGGIO_VIDEOFEEDTRANSFORMER_H
#define ARPEGGIO_VIDEOFEEDTRANSFORMER_H

#include <opencv2/opencv.hpp>
#include "KeyFinder.h"
#include "../rendering/Renderer.h"
#include <optional>
#include "CMarkerFinder.h"

using namespace cv;

class VideoFeedTransformer {
public:
    /**
     * Consumes frames of the provided feed, runs the processing required to recognize black keys, white keys, and c keys in KeyFinder nad CMarkerFinder and then calls onTransformed.
     * @param capture the video feed to be processed
     * @param onTransformed a callback, that takes in the original frame, the processed frame for white keys, and a reference to the keyfinder and cmakrerfinder classes.
     */
    void consumeFeed(VideoCapture capture, std::function<void(Mat, Mat, KeyFinder, CMarkerFinder)> onTransformed);

    /**
     *This is a convenience overload that also accepts a renderer that's called on every frame. See the other overload for usage.
     */
    void consumeFeed(VideoCapture capture,
                     std::function<void(Mat, Mat, KeyFinder, Renderer &renderer, CMarkerFinder)> onTransformed,
                     Renderer &renderer);

private:
    Mat processFrame(Mat source);

    KeyFinder keyFinder;
    CMarkerFinder markerFinder;
};


#endif //ARPEGGIO_VIDEOFEEDTRANSFORMER_H
