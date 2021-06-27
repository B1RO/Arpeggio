#include "./ContinuousMusic.h"
#include <chrono>

using namespace std;

namespace music {

    int ContinuousMusic::getTimeFromStart() {
        if(!started) {
            started = true;
            start();
        }

        chrono::steady_clock::time_point end = chrono::steady_clock::now();

        return chrono::duration_cast<chrono::milliseconds>(
            end - begin
        ).count();
    }

    std::chrono::steady_clock::time_point ContinuousMusic::start() {
        return begin = chrono::steady_clock::now();
    };

    vector<Note> ContinuousMusic::fromPointInTime(vector<Note> notes) {
        int ms = getTimeFromStart();
        vector<Note> output = {};

        for(Note note : notes) {

            if(ms >= get<1>(note)) { // note has already started
                if(ms < get<1>(note) + get<2>(note)) { // note is still playing
                    output.push_back(
                        make_tuple(get<0>(note), 0, get<1>(note)+get<2>(note)-ms, get<3>(note))
                    );
                }
            } else { // note will be played later
                output.push_back(
                    make_tuple(get<0>(note), get<1>(note)-ms, get<2>(note), get<3>(note))
                );
            }
        }

        return output;
    }
}