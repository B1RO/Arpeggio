#include <tuple>

#include "./ContinuousMusic.h"

using namespace std;
using namespace music;

namespace music {
    class CBlues : public ContinuousMusic {
    public:
        /**
         * The notes of the C blues scale. Goes upwards and then downwards
         */
        vector<Note> notes();
    };
}