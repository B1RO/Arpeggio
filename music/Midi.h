#include <tuple>
#include <chrono>

#include "./ContinuousMusic.h"

using namespace std;
using namespace music;

namespace music {
    /**
     * Class that is meant to be used for music that is played from a .mid file.
     **/
    class Midi : public ContinuousMusic {
    public:
        Midi(string path);
        /**
         * The notes imported the midi file
         */
        vector<Note> notes();
    protected:
        vector<Note> importedNotes;
    };
}