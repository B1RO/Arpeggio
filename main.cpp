#include "key-recognition/KeyFinder.h"
#include "key-recognition/VideoFeedTransformer.h"

void onTransformed(Mat original, Mat processed, KeyFinder keyFinder) {

    keyFinder.colorPianoKeysIntoFrame(processed);
    keyFinder.colorKey(processed, Pitch::C1, Scalar(255, 0, 255));
    keyFinder.labelKey(processed, Pitch::C1);
    keyFinder.colorKey(processed, Pitch::A1, Scalar(255, 0, 255));
    keyFinder.labelKey(processed, Pitch::A1);
    imshow("window", processed);
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