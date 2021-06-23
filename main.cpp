#include <iostream>
#include<opencv2/opencv.hpp>
#include <numeric>
#include "key-recognition/KeyFinder.h"


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
    out.push_back(contours[smallestDiffIndex - 1]);
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
    Size size1(384, 216);
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
        centroids.push_back(Point(cX,cY));
    }
    return centroids;
}




void highlightCKey(Mat frame, contour_vector_t keys, contour_vector_t markers) {
    contour_t leftMarker = findLeftYellowMarker(markers);
    for (int i = 0; i < keys.size(); i++) {
        if (pointPolygonTest(keys[i], leftMarker[0], false) > 0) {
            drawContours(frame, keys, i, Scalar(0, 255, 0), FILLED);
        }
    }
}

Mat thresholdYellowObjects(Mat frame) {
    Mat frame_HSV;
    Mat frame_threshold;
    cvtColor(frame, frame_HSV, COLOR_BGR2HSV);
    inRange(frame_HSV, Scalar(20, 100, 100), Scalar(30, 255, 255), frame_threshold);
    return frame_threshold;
}

int main() {
    VideoCapture cap("C:\\Code\\Arpeggio\\sample_video_1.mp4");

    namedWindow(original, WINDOW_NORMAL);

    if (!cap.isOpened()) {
        cout << "No video!" << endl;
        exit(0);
    }

    Mat frame;
    clock_t current_ticks, delta_ticks;
    clock_t fps = 0;
    KeyFinder keyFinder;
    while (cap.read(frame)) {
        current_ticks = clock();
        frame = downscaleFrame(frame);
        keyFinder.processFrame(frame);
//        keyFinder.colorPianoKeysIntoFrame(frame);

        contour_vector_t cMarkerContours = findYellowMarkers(frame);
        keyFinder.specifyCs(getCentroidsFromContours(cMarkerContours));
        keyFinder.__debug__renderCCentroids(frame);
        keyFinder.colorKey(frame, 'C', Scalar(255,0,255));
        keyFinder.labelKey(frame, 'C');
        keyFinder.colorKey(frame, 'A', Scalar(255,0,255));
        keyFinder.labelKey(frame, 'A');
//        drawContours(frame, yellowMarkers, -1, Scalar(255, 0, 0));

        imshow(original, frame);


        int key = waitKey(60);
        if (key == 27)
            break;

        delta_ticks = clock() - current_ticks; //the time, in ms, that took to render the scene
        if (delta_ticks > 0)
            fps = CLOCKS_PER_SEC / delta_ticks;
        cout << fps << endl;
    }

    return 0;
}