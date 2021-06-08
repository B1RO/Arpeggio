#include <iostream>
#include <vector>

#include "./Note.cpp"

using namespace std;
using namespace music;

namespace music {
    /**
     * Objects of Music classes provide notes, eg for displaying on a keyboard
     */
    class Music {
    public:
        /**
         * Returns a vector of all notes that will at some point be played.
         * Depending on the implementation, the return value might vary
         * depending on realtime events, so this should be called regularly
         * to update.
         */
        virtual vector<Note> notes() = 0;
    };
}
