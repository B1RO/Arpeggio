//
// Created by marti on 6/22/2021.
//
#ifndef ARPEGGIO_KEYFINDER_H
#define ARPEGGIO_KEYFINDER_H

#include<opencv2/opencv.hpp>
#include "./../music/Pitch.h"
#include <optional>

using namespace cv;
using namespace std;

typedef vector<Point> contour_t;
typedef vector<contour_t> contour_vector_t;

class KeyFinder {
public:
    Mat processFrame(Mat frame);
    /**
     * Processes the source image into a form that's suitable for the extraction of white key contours and extracts them.
     * This method stores the white key contours as a side effect.
     * @warning this method needs to be called before using methods that interact with the white keys.
     * @param frame The source RGB Matrix
     * @return the processed matrix
     */
    Mat processFrameBlackKeys(Mat frame);
    /**
     * A utility function that colors each of the white piano keys contours, the color is chosen based on the Pitch
     * @param frame The source RGB Matrix
     */
    void colorPianoKeysIntoFrame(Mat frame);
    /**
     * A utility function that colors each of the black piano keycontours, the color is chosen randomly
     * @param frame The source RGB Matrix
     */
    void colorBlackPianoKeysIntoFrame(Mat frame);
    void __debug__renderCCentroids(Mat frame);
    void __debug__renderPianoSquares(Mat frame);
    void __debug__renderPianoRectangles(Mat frame);
    /**
     * This simply sets the C key positions for further processing.
     * @warning this method needs to be called before any functions that use position of the C keys.
     * @param CKeys
     */
    void specifyCs(vector<Point> CKeys);
    /**
     * Colors a key into the frame based on the pitch. If the key cannot be recognized and/or found, nothing is done.
     * @warning The coloring is done in-place and the frame is modified
     * @warning This uses the currently stored black and white contours, as such processFrameKeys and processFrameBlackKeys should ideally have been called before this function.
     * @param frame The source RGB Matrix
     * @param pitch The pitch that is to be colored onto the piano.
     * @param color color that will be used
     */
    void colorKey(Mat frame, Pitch pitch, Scalar color);
    /**
     * Renders a text over the specified key. If the key cannot be recognized and/or found, nothing is done.
    * @param frame The source RGB Matrix
     * @param pitch The pitch of the key that is to be labeled.
     */
    void labelKey(Mat frame, Pitch pitch);
    /**
     * Computes the position of the centroid of the contour of the given key.
     * @param pitch the pitch of the contour, whose centroid you wish to compute
     * @return position if the contour is found, nullopt otherwise
     */
    optional<Point> getKeyCentroid(Pitch pitch);
    /**
     * Returns the contour corresponding to the given key
     * @param pitch the pitch of the desired contour
     * @return contour if found, nullopt otherwise
     */
    optional<contour_t> getKeyContour(Pitch pitch);
    /**
     * Fits a square onto the bottom of a white key for pose estimation. The bottom edge of the square is the original edge of the contour.
     * The left and right edges are computed by creating a vector in the direction of the original left/right edges, but shortening the length to  equal the length f the bottom edge.
     * @param contour the contour to fit the square onto, can be obtained by getKeyContour
     * @return a square;
     */
    static contour_t molestPianoKeyIntoASquare(contour_t contour);
    /**
     * See molestPianokeyIntoASquare
     */
    static contour_t molestPianoKeyIntoARectangle(contour_t contour);
    /**
     * Helper function that returns the lits of contours reshaped into squares via the molestPianoIntoASquare function
     * @return a vector of square contours
     */
    vector<contour_t> getKeysAsSquares();
    /**
     * see getKeysAsRectangles
     */
    vector<contour_t> getKeysAsRectangles();
private:
    Mat sourceFrame;
    int getIndexOfContourClosestToPoint(Point point, contour_vector_t contours);
    std::vector<Point> yellowMarkers;
    std::vector<contour_t> keyContours;
    //I don't own a piano please don't judge me I don't know how they are called.I am sure the left leftmost black key is called Josh though.
    std::vector<contour_t> blackKeyContours;
    static Mat thresholdKeys(Mat frame);
    static Mat thresholdKeysGrayscale(Mat frame);
    static Mat thresholdKeysBlack(const Mat& frame);
    static Mat dilateKeys(Mat frame);

    static Mat getPianoMask(const Mat &frame);
};


#endif //ARPEGGIO_KEYFINDER_H
