#pragma once
#include <tuple>
#include <chrono>

#include "./Music.h"

using namespace std;
using namespace music;

namespace music {
    /**
     * Class that is meant to be used for music that should be played without interruptions.
     **/
    class ContinuousMusic : public Music {
    public:
        /**
         * Time from the first {getNotes} call in ms
         **/
        int getTimeFromStart();
    protected:
        /**
         * Sets the current time as start time for the music. Should be called in
         * getNotes.
         * @return current time which is the start point of the played music
         **/
        chrono::steady_clock::time_point start();
        chrono::steady_clock::time_point begin;
        bool started = false;
        /**
         * converts notes with absolute times (from the song's start) to notes
         * with time values relative to the given time progression
         **/
        vector<Note> fromPointInTime(vector<Note> notes);
    };
}