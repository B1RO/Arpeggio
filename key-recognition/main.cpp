#include <iostream>
#include<opencv2/opencv.hpp>


using namespace cv;

Mat frame, bw, dst, detected_edges;

typedef std::vector<Point> contour_t;
typedef std::vector<contour_t> contour_vector_t;

char* original = "Original";
char* processed = "Processed";

int lowThreshold = 20;
const int max_lowThreshold = 100;
const int ratio = 3;
const int kernel_size = 3;


static void CannyThreshold(int, void*)
{
//    blur( bw, detected_edges, Size(3,3) );
    bilateralFilter(bw, detected_edges, 5, 50, 50);
    Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );
    dst = Scalar::all(0);
    frame.copyTo( dst, detected_edges);
}

int main() {
    VideoCapture cap("/Users/tobias/tum/ar/project/Arpeggio/sample_video.mp4");

    namedWindow(original);
    namedWindow(processed);

    createTrackbar( "Min Threshold:", processed, &lowThreshold, max_lowThreshold, nullptr );

    if (!cap.isOpened()) {
        std::cout << "No video!" << std::endl;
        exit(0);
    }

    while (cap.read(frame)) {
        dst.create(frame.size(), frame.type());

        cvtColor(frame, bw, CV_BGR2GRAY);

//        CannyThreshold(0, 0); // can be used alternatively instead of regular threshold

        threshold(bw, detected_edges, 75, 255, THRESH_BINARY); // 75 for somewhat combined white area, 150 for more individual keys

        contour_vector_t contours;
        findContours(detected_edges, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

        for (int i = 0; i < contours.size(); i++) {
            std::vector<Point> approx_contour;
            approxPolyDP(contours[i], approx_contour, arcLength(contours[i], true) * 0.02, true);
            if (approx_contour.size() > 1) {
                drawContours(frame, contours, i, Scalar(rand() & 256, rand() % 256, rand() % 256), FILLED );
            }
        }
//
//        imshow(processed, dst);
        imshow(original, frame);


        int key = waitKey(60);
        if (key == 27)
            break;
    }

    return 0;
}