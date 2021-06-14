#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <numeric>

using namespace cv;
const int max_value_H = 360 / 2;
const int max_value = 255;
const String window_capture_name = "Video Capture";
const String window_detection_name = "Object Detection";


Mat threshholdWithAdaptative(Mat frame) {
    Mat out;
    cvtColor(frame, out, COLOR_BGR2GRAY);
    adaptiveThreshold(out, out, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 13, 5);
    return out;
}


Mat threshholdWithHSV(Mat frame) {
    Mat frame_HSV;
    Mat frame_threshold;
    cvtColor(frame, frame_HSV, COLOR_BGR2HSV);
    int low_H = 10, low_S = 0, low_V = 200;
    int high_H = max_value_H, high_S = max_value, high_V = max_value;
    inRange(frame_HSV, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), frame_threshold);
    return frame_threshold;
}

std::vector<std::vector<Point>> getApproxContours(Mat processedFrame) {
    std::vector<std::vector<Point>> approxContours;
    std::vector<std::vector<Point> > contours;
    std::vector<Vec4i> hierarchy;
    findContours(processedFrame, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
    for (int i = 0; i < contours.size(); i++) {
        std::vector<Point> approx_contour;
        approxPolyDP(contours[i], approx_contour, arcLength(contours[i], true) * 0.02, true);
        if (approx_contour.size() > 2 && contourArea(approx_contour) > 500) {
            approxContours.push_back(contours[i]);
        }
    }
    return approxContours;
}

void colorPianoKeys(Mat frame, std::vector<std::vector<Point>> contours) {
    for (int i = 0; i < contours.size(); i++) {
        drawContours(frame, contours, i, Scalar(rand() & 256, rand() % 256, rand() % 256), FILLED);
    }
}

std::vector<double> getAreas(std::vector<std::vector<Point>> contours) {
    std::vector<double> areas(contours.size());
    for (int i = 0; i < contours.size(); i++) {
        areas.push_back(contourArea(contours[i]));
    }
    return areas;
}

double getAverageArea(std::vector<double> areas) {
    return std::accumulate(areas.begin(), areas.end(), 0.0) / areas.size();
}

double getAreaStdDev(std::vector<double> areas) {
    double mean = getAverageArea(areas);
    double sq_sum = std::inner_product(areas.begin(), areas.end(), areas.begin(), 0.0);
    double stdev = std::sqrt(sq_sum / areas.size() - mean * mean);
    return stdev;
}

std::vector<std::vector<Point>>
getContoursWithAreaCloseWithinMean(std::vector<std::vector<Point>> in, double mean, double stdev) {
    std::vector<std::vector<Point>> out;
    for (const auto &item : in) {
        if (abs(contourArea(item) - mean) < stdev / 2) {
            out.push_back(item);
        }
    }
    return out;
}

std::vector<double> getKeyCentroidsXSorted(std::vector<std::vector<Point>> contours) {
    std::vector<double> xs;
    for (const auto &item : contours) {
        auto M = moments(item);
        auto cX = int(M.m10 / M.m00);
        xs.push_back(cX);
    }
    std::sort(xs.begin(), xs.end());
    return xs;
}

int dilation_size = 20;

int main(int argc, char *argv[]) {
    VideoCapture cap("C:\\Code\\Arpeggio\\sample_video.mp4");
    namedWindow(window_detection_name, WINDOW_NORMAL);
    // Trackbars to set thresholds for HSV values
    Mat frame;
    double stdDev = 0;
    std::vector<std::vector<Point>> persistedContours;
    while (true) {
        cap >> frame;
        if (frame.empty()) {
            break;
        }
        // Convert from BGR to HSV colorspace
        // Detect the object based on HSV Range Values
        // Show the frames

        Mat thresholded = threshholdWithHSV(frame);
        Mat element = getStructuringElement(MORPH_RECT,
                                            Size(2 * dilation_size + 1, 2 * dilation_size + 1),
                                            Point(dilation_size, dilation_size));
        Mat eroded;
        erode(thresholded, eroded, element);
        auto contours = getApproxContours(eroded);
        double averageArea = getAverageArea(getAreas(contours));
        stdDev = getAreaStdDev(getAreas(contours));
        auto contoursWithinMean = getContoursWithAreaCloseWithinMean(contours, averageArea, stdDev);
        colorPianoKeys(frame, contoursWithinMean);

        imshow(window_detection_name, frame);
        char key = (char) waitKey(30);
        if (key == 'q' || key == 27) {
            break;
        }
    }
    return 0;
}