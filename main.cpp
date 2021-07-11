#include "key-recognition/KeyFinder.h"
#include "key-recognition/VideoFeedTransformer.h"

void onTransformed(Mat original, Mat processed, KeyFinder keyFinder, CMarkerFinder markerFinder) {

    keyFinder.colorKey(original, Pitch::C1, Scalar(255, 0, 255));
//    keyFinder.labelKey(original, Pitch::C1);
    keyFinder.colorKey(original, Pitch::Eflat7, Scalar(255, 0, 255));
//    keyFinder.labelKey(original, Pitch::A1);
//    markerFinder.highlightMarkers(original);
//    keyFinder.__debug__renderPianoSquares(original);
    imshow("window", original);
}

int main() {
    VideoCapture cap("C:\\Code\\Arpeggio\\sample_video_1.mp4");
    namedWindow("window", WINDOW_NORMAL);
    if (!cap.isOpened()) {
        cout << "No video!" << endl;
        exit(0);
    }
    VideoFeedTransformer transformer;
    transformer.consumeFeed(cap, onTransformed);
    return 0;
}
