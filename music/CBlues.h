#include <tuple>

#include "./Music.h"

using namespace std;
using namespace music;

namespace music {
    class CBlues : public Music {
    public:
        /**
         * The notes of the C blues scale. Starts with all at once,
         * then ordered upwards and downwards
         */
        vector<Note> notes();
    };
}