//
// Created by marti on 6/26/2021.
//

#ifndef ARPEGGIO_CMARKERFINDER_H
#define ARPEGGIO_CMARKERFINDER_H


#include "KeyFinder.h"

class CMarkerFinder {
public:
    /**
     * Finds the C markers a given frame and stores them internally for later usage
     * @param frame The source RGB Matrix
     */
    void processFrame(Mat frame);
    /**
     * @warning processFrame should be called before this method
     * @return the found yellow markers, sorted along the x axis.
     */
    vector<contour_t> getYellowMarkers();
    /**
     * renders these markers into the original frame
     * @param frame The source RGB Matrix
     */
    void highlightMarkers(Mat frame);
private:
    Mat sourceFrame;
    vector<contour_t> yellowMarkers;
    void findYellowMarkers(Mat frame);
};


#endif //ARPEGGIO_CMARKERFINDER_H
