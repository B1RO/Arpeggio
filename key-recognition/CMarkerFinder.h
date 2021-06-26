//
// Created by marti on 6/26/2021.
//

#ifndef ARPEGGIO_CMARKERFINDER_H
#define ARPEGGIO_CMARKERFINDER_H


#include "KeyFinder.h"

class CMarkerFinder {
public:
    void processFrame(Mat frame);
    vector<contour_t> getYellowMarkers();
    void highlightMarkers(Mat frame);
private:
    vector<contour_t> yellowMarkers;
    void findYellowMarkers(Mat frame);
};


#endif //ARPEGGIO_CMARKERFINDER_H
