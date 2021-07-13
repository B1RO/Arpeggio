#include <tuple>

#include "./ContinuousMusic.h"

using namespace std;
using namespace music;

namespace music {
    class CDur : public ContinuousMusic {
    public:
        /**
         * The notes of the C major scale. Goes upwards and then downwards.
         */
        vector<Note> notes();
    };
}