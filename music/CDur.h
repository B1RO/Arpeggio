#include <tuple>

#include "./ContinuousMusic.h"

using namespace std;
using namespace music;

namespace music {
    class CDur : public ContinuousMusic {
    public:
        /**
         * The notes of the C major scale. Starts with all at once,
         * then ordered upwards and downwards
         */
        vector<Note> notes();
    };
}