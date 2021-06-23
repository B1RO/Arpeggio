#include <iostream>
#include<opencv2/opencv.hpp>
#include <numeric>


using namespace cv;
using namespace std;

//Mat frame, bw, dst, detected_edges, markers;

typedef vector<Point> contour_t;
typedef vector<contour_t> contour_vector_t;

char *original = "Original";
char *processed = "Processed";
char *yellow = "Yellow";

int lowThreshold = 20;
const int max_lowThreshold = 100;
const int t_ratio = 3;
const int kernel_size = 3;
int erosion_size = 2;

int iLowH = 22;
int iHighH = 38;


//static void CannyThreshold(int, void *) {
//    blur( bw, detected_edges, Size(3,3) );
//    bilateralFilter(bw, detected_edges, 5, 50, 50);
//    Canny(detected_edges, detected_edges, lowThreshold, lowThreshold * t_ratio, kernel_size);
//    dst = Scalar::all(0);
//    frame.copyTo(dst, detected_edges);
//}

vector<contour_t> findYellowMarkers(Mat frame) {
    Mat imgHSV;
    cvtColor(frame, imgHSV, COLOR_BGR2HSV);
    inRange(imgHSV, Scalar(iLowH, 160, 60), Scalar(iHighH, 255, 255), imgHSV);
//    Mat element = getStructuringElement(MORPH_RECT,
//                                        Size(2 * erosion_size + 1, 2 * erosion_size + 1),
//                                        Point(erosion_size, erosion_size));
//    morphologyEx(detected_edges, detected_edges, 2, element);
//    morphologyEx(detected_edges, detected_edges, 3, element);
    contour_vector_t contours;
    findContours(imgHSV, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
    return contours;
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

void colorPianoKeys(Mat frame, contour_vector_t contours) {
    for (int i = 0; i < contours.size(); i++) {
        drawContours(frame, contours, i, Scalar(rand() & 256, rand() % 256, rand() % 256), FILLED);
    }
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

vector<double> getKeyCentroidsXSorted(contour_vector_t contours) {
    vector<double> xs;
    for (const auto &item : contours) {
        auto M = moments(item);
        auto cX = int(M.m10 / M.m00);
        xs.push_back(cX);
    }
    sort(xs.begin(), xs.end());
    return xs;
}

vector<contour_t> findKeys(Mat bw) {
    contour_vector_t allContours;
    findContours(bw, allContours, RETR_LIST, CHAIN_APPROX_SIMPLE);

    contour_vector_t filteredContours;
    for (int i = 0; i < allContours.size(); i++) {
        if (contourArea(allContours[i]) > 5000) {
            filteredContours.push_back(allContours[i]);
        }
    }

//    for (int i = 0; i < allContours.size(); i++) {
//        vector<Point> approx_contour;
//        approxPolyDP(allContours[i], allContours[i], arcLength(allContours[i], true) * 0.02, true);
//    }
    contour_vector_t contoursWithinMean = getContoursWithAreaCloseWithinMean(filteredContours);
    return contoursWithinMean;

}

void highlightCKey(Mat frame, contour_vector_t keys, contour_vector_t markers) {
    contour_t leftMarker = findLeftYellowMarker(markers);
    for (int i = 0; i < keys.size(); i++) {
        if (pointPolygonTest(keys[i], leftMarker[0], false) > 0) {
            drawContours(frame, keys, i, Scalar(0, 255, 0), FILLED);
        }
    }
}

int main() {
    VideoCapture cap("/Users/tobias/tum/ar/project/Arpeggio/video_1_1080.mov");

    namedWindow(original);
    namedWindow(processed);
    namedWindow(yellow);

//    createTrackbar( "Min Threshold:", processed, &lowThreshold, max_lowThreshold, nullptr );
    cvCreateTrackbar("LowH", yellow, &iLowH, 179); //Hue (0 - 179)
    cvCreateTrackbar("HighH", yellow, &iHighH, 179);

    if (!cap.isOpened()) {
        cout << "No video!" << endl;
        exit(0);
    }

    Mat frame;
    while (cap.read(frame)) {
//        dst.create(frame.size(), frame.type());


//        CannyThreshold(0, 0); // can be used alternatively instead of regular threshold
//        bitwise_not(detected_edges, detected_edges);

//        Mat element = getStructuringElement( MORPH_RECT,
//                                             Size( 2*erosion_size + 1, 2*erosion_size+1 ),
//                                             Point( erosion_size, erosion_size ) );
//        morphologyEx( detected_edges, detected_edges,2, element );
//        morphologyEx( detected_edges, detected_edges,3, element );
//        erode( detected_edges, detected_edges, element );
//        vector<Vec2f> lines;
//        HoughLines(detected_edges, lines, 1, CV_PI/180, 150, 0, 0 ); // runs the actual detection
//        for( size_t i = 0; i < lines.size(); i++ )
//        {
//            float rho = lines[i][0], theta = lines[i][1];
//            Point pt1, pt2;
//            double a = cos(theta), b = sin(theta);
//            double x0 = a*rho, y0 = b*rho;
//            pt1.x = cvRound(x0 + 1000*(-b));
//            pt1.y = cvRound(y0 + 1000*(a));
//            pt2.x = cvRound(x0 - 1000*(-b));
//            pt2.y = cvRound(y0 - 1000*(a));
//            line( frame, pt1, pt2, Scalar(0,0,255), 3, LINE_AA);
//        }

        // Probabilistic Line Transform
//        vector<Vec4i> linesP; // will hold the results of the detection
//        HoughLinesP(detected_edges, linesP, 1, CV_PI/180, 50, 50, 10 ); // runs the actual detection
//        // Draw the lines
//        for( size_t i = 0; i < linesP.size(); i++ )
//        {
//            Vec4i l = linesP[i];
//            line( frame, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,255,0), 3, LINE_AA);
//        }

//        contour_vector_t contours;
//        findContours(detected_edges, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
//
//        for (int i = 0; i < contours.size(); i++) {
//            vector<Point> approx_contour;
//            approxPolyDP(contours[i], approx_contour, arcLength(contours[i], true) * 0.02, true);
//            if (approx_contour.size() > 1) {
//                drawContours(frame, contours, i, Scalar(rand() & 256, rand() % 256, rand() % 256), FILLED );
//            }
//        }

        Mat bw = preprocessFrame(frame);
        contour_vector_t keys = findKeys(bw);
//        drawContours(frame, keys, -1, Scalar(0, 0, 255), FILLED);
//        colorPianoKeys(frame, keys);

        contour_vector_t yellowMarkers = findYellowMarkers(frame);
        drawContours(frame, yellowMarkers, -1, Scalar(255, 0, 0));
        highlightCKey(frame, keys, yellowMarkers);

//        imshow(processed, bw);
        imshow(original, frame);


        int key = waitKey(60);
        if (key == 27)
            break;
    }

    return 0;
}