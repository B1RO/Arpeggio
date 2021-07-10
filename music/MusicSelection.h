#include <iostream>
#include <cmath>
#include <vector>
#include "Pitch.h"
#include "Music.h"

using namespace std;

namespace music {
    class MusicSelection {
    public:
        static tuple<Music*, bool> selectMusic();
    };
}
