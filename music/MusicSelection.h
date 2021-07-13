#include <iostream>
#include <cmath>
#include <vector>
#include "Pitch.h"
#include "Music.h"

using namespace std;

namespace music {
    class MusicSelection {
    public:
        /**
         * Allows Selection of a music class, so that a specific set of notes can be played.
         * MicPitchParser is disabled by default, uncomment all relating lines to add it.
         * @return {tuple<Music*, bool>} a pointer to a object of a Music implementation that
         *   shall be used to call notes() to obtain the notes that should be played and a
         *   boolean that indicates whether advanced mode is used
         **/
        static tuple<Music*, bool> selectMusic();
    };
}
