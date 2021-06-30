//
// Created by marti on 6/30/2021.
//

#ifndef ARPEGGIO_CONTOURSCALER_H
#define ARPEGGIO_CONTOURSCALER_H


#include <vector>
#include<opencv2/opencv.hpp>

class ContourScaler {
public:
    static std::vector<std::vector<cv::Point>> scaleContours(int sourceWidth, int sourceHeight, int targetWidth,
                                                      int targetHeight,
                                                      std::vector<std::vector<cv::Point>> sourceContours) {
        std::vector<std::vector<cv::Point>> targetContours;
        std::transform(sourceContours.begin(), sourceContours.end(), std::back_inserter(targetContours),
                       [sourceWidth, sourceHeight, targetWidth, targetHeight](auto x) {
                           return ContourScaler::scaleContour(sourceWidth, sourceHeight, targetWidth, targetHeight, x);
                       });
        return targetContours;
    }

    static std::vector<cv::Point> scaleContour(int sourceWidth, int sourceHeight, int targetWidth, int targetHeight,
                                        std::vector<cv::Point> sourceContour) {
        std::vector<cv::Point> targetContour;
        std::transform(sourceContour.begin(), sourceContour.end(), std::back_inserter(targetContour),
                       [sourceWidth, sourceHeight, targetWidth, targetHeight](const cv::Point &point) {
                           return cv::Point(point.x * targetWidth / sourceWidth, point.y * targetHeight / sourceHeight);
                       });
        return targetContour;
    }

};


#endif //ARPEGGIO_CONTOURSCALER_H
