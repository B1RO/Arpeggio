//
// Created by marti on 6/22/2021.
//

#include "KeyFinder.h"
#include "ContourScaler.h"
#include <iostream>
#include <functional>
#include <vector>
#include <numeric>


/**
 * This is the reason WHY YOU DON'T USE ENUMS  :-D
 * @param pitch
 * @return
 */
char convertPitchToChar(Pitch pitch) {
    if (pitch == Pitch::A0 || pitch == Pitch::A1 || pitch == Pitch::A2 ||
        pitch == Pitch::A3 || pitch == Pitch::A4 || pitch == Pitch::A5 ||
        pitch == Pitch::A6 || pitch == Pitch::A7) {
        return 'A';
    } else if (pitch == Pitch::B0 || pitch == Pitch::B1 || pitch == Pitch::B2 ||
               pitch == Pitch::B3 || pitch == Pitch::B4 || pitch == Pitch::B5 || pitch == Pitch::B6 ||
               pitch == Pitch::B7) {
        return 'B';
    } else if (pitch == Pitch::C1 || pitch == Pitch::C2 ||
               pitch == Pitch::C3 || pitch == Pitch::C4 || pitch == Pitch::C5 || pitch == Pitch::C6 ||
               pitch == Pitch::C7 || pitch == Pitch::C8) {
        return 'C';
    } else if (pitch == Pitch::D1 || pitch == Pitch::D2 ||
               pitch == Pitch::D3 || pitch == Pitch::D4 || pitch == Pitch::D5 || pitch == Pitch::D6 ||
               pitch == Pitch::D7) {
        return 'D';
    } else if (pitch == Pitch::E1 || pitch == Pitch::E2 ||
               pitch == Pitch::E3 || pitch == Pitch::E4 || pitch == Pitch::E5 || pitch == Pitch::E6 ||
               pitch == Pitch::E7) {
        return 'E';
    } else if (pitch == Pitch::F1 || pitch == Pitch::F2 ||
               pitch == Pitch::F3 || pitch == Pitch::F4 || pitch == Pitch::F5 || pitch == Pitch::F6 ||
               pitch == Pitch::F7) {
        return 'F';
    } else if (pitch == Pitch::G1 || pitch == Pitch::G2 ||
               pitch == Pitch::G3 || pitch == Pitch::G4 || pitch == Pitch::G5 || pitch == Pitch::G6 ||
               pitch == Pitch::G7) {
        return 'G';
    }
    return 'X';
}

Point getContourCentroid(contour_t contour) {
    auto M = moments(contour);
    auto cX = int(M.m10 / M.m00);
    auto cY = int(M.m01 / M.m00);
    return Point(cX, cY);
}

Mat KeyFinder::processFrame(Mat frame) {
    this->sourceFrame = frame;
    auto processedFrame = dilateKeys(thresholdKeys(frame));
    contour_vector_t allContours;
    findContours(processedFrame, allContours, RETR_LIST, CHAIN_APPROX_SIMPLE);

    contour_vector_t filteredContours;
    for (int i = 0; i < allContours.size(); i++) {
//        contour_t approx;
//        approxPolyDP(allContours[i], approx, arcLength(allContours[i], true) * 0.02, true);
//        if (approx.size() >= 4 && contourArea(approx) / (frame.rows * frame.cols) >= 0.005)
//            filteredContours.push_back(approx);
        if (allContours[i].size() > 4 && contourArea(allContours[i]) / (frame.rows * frame.cols) >= 0.005)
            filteredContours.push_back(allContours[i]);
    }

    //sort contours by centroid x position
    sort(filteredContours.begin(), filteredContours.end(),
         [](const contour_t &a, const contour_t &b) -> bool {
             return getContourCentroid(a).x < getContourCentroid(b).x;
         });

    this->keyContours = filteredContours;
    return processedFrame;
}

const int max_value_H = 360 / 2;
const int max_value = 255;

Mat KeyFinder::thresholdKeys(Mat frame) {
    Mat frame_HSV;
    Mat frame_threshold;
    cvtColor(frame, frame_HSV, COLOR_BGR2HSV);
    int low_H = 0, low_S = 0, low_V = 195;
    int high_H = max_value_H, high_S = max_value, high_V = max_value;
    inRange(frame_HSV, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), frame_threshold);
    return frame_threshold;
}


Mat KeyFinder::thresholdKeysBlack(const Mat &frame) {
    cv::Mat invSrc = cv::Scalar::all(255) - frame;
    Mat frame_HSV;
    Mat frame_threshold;
    cvtColor(invSrc, frame_HSV, COLOR_BGR2HSV);
    int low_H = 0, low_S = 0, low_V = 242;
    int high_H = max_value_H, high_S = max_value, high_V = max_value;
    inRange(frame_HSV, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), frame_threshold);
    return frame_threshold;
}

Mat KeyFinder::dilateKeys(Mat frame) {
    int dilation_size = frame.cols / 200;
    Mat element = getStructuringElement(MORPH_RECT,
                                        Size(2 * dilation_size + 1, 2 * dilation_size + 1),
                                        Point(dilation_size, dilation_size));
    Mat eroded;
    erode(frame, eroded, element);
    return eroded;
}


int convertPitchToCOffset(Pitch pitch) {
    if (pitch < Pitch::A1) {
        return -3;
    } else if (pitch < Pitch::A2) {
        return -2;
    } else if (pitch < Pitch::A3) {
        return -1;
    } else if (pitch < Pitch::A4) {
        return 0;
    } else if (pitch < Pitch::A5) {
        return 1;
    } else if (pitch < Pitch::A6) {
        return 2;
    } else if (pitch < Pitch::A7) {
        return 3;
    } else {
        return 4;
    }
}


void KeyFinder::specifyCs(vector<Point> CKeys) {
    this->yellowMarkers = std::move(CKeys);
}

void KeyFinder::__debug__renderCCentroids(Mat frame) {
    for (const auto &item : this->yellowMarkers) {
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

void KeyFinder::colorKey(Mat frame, Pitch pitch, Scalar color) {
    vector<Scalar> keyColors = {{183, 28, 28},
                                {136, 14, 79},
                                {74,  20, 140},
                                {26,  35, 126},
                                {1,   87, 155},
                                {0,   77, 64}};
    int offset = convertPianoKeyToOffsetFromC(convertPitchToChar(pitch));
    for (const auto &item : this->yellowMarkers) {
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

void KeyFinder::labelKey(Mat frame, Pitch pitch) {
    int offset = convertPianoKeyToOffsetFromC(convertPitchToChar(pitch));
    for (const auto &item : this->yellowMarkers) {
        auto contourIndex = this->getIndexOfContourClosestToPoint(item);
        auto contour = this->keyContours[contourIndex + offset];
        auto centroid = getContourCentroid(contour);
        putText(frame, std::string(1, convertPitchToChar(pitch)), centroid - Point(5, -5), FONT_HERSHEY_DUPLEX, 0.5,
                CV_RGB(255, 0, 0), 1);
    }
}

std::optional<contour_t> KeyFinder::getKeyContour(Pitch pitch) {
    int offset = convertPianoKeyToOffsetFromC(convertPitchToChar(pitch));
    for (const auto &item : this->yellowMarkers) {
        auto contourIndex = this->getIndexOfContourClosestToPoint(item);
        if (contourIndex + offset < this->keyContours.size()) {
            auto contour = this->keyContours[contourIndex + offset];
            return std::optional<contour_t>(contour);
        }
        return std::nullopt;
    }
}

std::optional<Point> KeyFinder::getKeyCentroid(Pitch pitch) {
    auto contour = getKeyContour(pitch);
    if (contour.has_value())
        return getContourCentroid(contour.value());
    return std::nullopt;
}

Mat KeyFinder::thresholdKeysGrayscale(Mat frame) {
    Mat bw;
    cvtColor(frame, bw, CV_BGR2GRAY);
    threshold(bw, bw, 150, 255,
              THRESH_BINARY); // 75 for somewhat combined white area, 150 for more individual keys
    return bw;
}

contour_t KeyFinder::molestPianoKeyIntoASquare(contour_t contour) {
    auto rect = minAreaRect(contour);
    vector<Point2f> boxPts(4);
    rect.points((boxPts.data()));
    vector<Point> out(4);
    std::transform(boxPts.begin(), boxPts.end(), out.begin(),
                   [](Point2f point) { return static_cast<cv::Point>(point); });
    auto bl = *std::min_element(contour.begin(), contour.end(),
                                [](const Point &a, const Point &b) { return b.x - a.x + a.y - b.y > 0; });
    auto br = *std::min_element(contour.begin(), contour.end(),
                                [](const Point &a, const Point &b) { return a.x - b.x + a.y - b.y > 0; });
    auto tl = *std::min_element(contour.begin(), contour.end(),
                                [](const Point &a, const Point &b) { return b.x - a.x + b.y - a.y > 0; });
    auto bottomEdge = br - bl;
    auto leftEdge = tl - bl;
    double leftEdgeNormInv = leftEdge.x == 0 && leftEdge.y == 0 ? 1 : 1 / norm(leftEdge);
    return {bl, br, br + leftEdge * leftEdgeNormInv * norm(bottomEdge),
            bl + leftEdge * leftEdgeNormInv * norm(bottomEdge)};
}

void KeyFinder::__debug__renderPianoSquares(Mat frame) {
    for (const auto &item : this->keyContours) {
        auto square = this->molestPianoKeyIntoASquare(item);
        auto scaled = ContourScaler::scaleContour(this->sourceFrame.cols, this->sourceFrame.rows, frame.cols,
                                                  frame.rows, square);
        drawContours(frame, vector<vector<Point> >(1, scaled), -1, Scalar(rand() & 256, rand() % 256, rand() % 256),
                     FILLED);
    }
}

contour_t KeyFinder::molestPianoKeyIntoARectangle(contour_t contour) {
    auto rect = minAreaRect(contour);
    vector<Point2f> boxPts(4);
    rect.points((boxPts.data()));
    vector<Point> out(4);
    std::transform(boxPts.begin(), boxPts.end(), out.begin(),
                   [](Point2f point) { return static_cast<cv::Point>(point); });
    return out;
}


void KeyFinder::__debug__renderPianoRectangles(Mat frame) {
    for (const auto &item : this->keyContours) {
        auto square = this->molestPianoKeyIntoARectangle(item);
        drawContours(frame, vector<vector<Point> >(1, square), -1, Scalar(rand() & 256, rand() % 256, rand() % 256),
                     FILLED);
    }
}

vector<contour_t> KeyFinder::getKeysAsSquares() {
    std::vector<contour_t> out(this->keyContours.size());
    transform(this->keyContours.begin(), this->keyContours.end(), back_inserter(out),
              molestPianoKeyIntoASquare);
    return out;
}

vector<contour_t> KeyFinder::getKeysAsRectangles() {
    std::vector<contour_t> out(this->keyContours.size());
    transform(this->keyContours.begin(), this->keyContours.end(), back_inserter(out),
              [this](auto contour) { return molestPianoKeyIntoARectangle(contour); });
    return out;
}


vector<double> getAreas(contour_vector_t contours) {
    vector<double> areas(contours.size());
    for (int i = 0; i < contours.size(); i++) {
        areas.push_back(contourArea(contours[i]));
    }
    return areas;
}

double getAverageArea(vector<double> areas) {
    return std::accumulate(areas.begin(), areas.end(), 0.0) / areas.size();
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
        if (abs(contourArea(item) - average) < stddev / 4) {
            out.push_back(item);
        }
    }
    return out;
}

Mat KeyFinder::processFrameBlackKeys(Mat frame) {
    this->sourceFrame = frame;
    auto dilation_size = 1;
    Mat element = getStructuringElement(MORPH_RECT,
                                        Size(2 * dilation_size + 1, 2 * dilation_size + 1),
                                        Point(dilation_size, dilation_size));
    auto processedFrame = thresholdKeysBlack(frame);
    contour_vector_t allContours;
    findContours(processedFrame, allContours, RETR_LIST, CHAIN_APPROX_SIMPLE);

    contour_vector_t filteredContours;
    for (int i = 0; i < allContours.size(); i++) {
        if (allContours[i].size() > 4 && contourArea(allContours[i]) / (frame.rows * frame.cols) >= 0.005)
            filteredContours.push_back(allContours[i]);
    }
    filteredContours = getContoursWithAreaCloseWithinMean(filteredContours);

    //sort contours by centroid x position
    sort(filteredContours.begin(), filteredContours.end(),
         [](const contour_t &a, const contour_t &b) -> bool {
             return getContourCentroid(a).x < getContourCentroid(b).x;
         });

    this->blackKeyContours = filteredContours;
    return processedFrame;
}

void KeyFinder::colorBlackPianoKeysIntoFrame(Mat frame) {
    auto scaled = ContourScaler::scaleContours(this->sourceFrame.cols, this->sourceFrame.rows, frame.cols, frame.rows,
                                               this->blackKeyContours);

    for (int i = 0; i < scaled.size(); i++) {
        drawContours(frame, scaled, i, Scalar(rand() & 256, rand() % 256, rand() % 256), FILLED);
    }
}


