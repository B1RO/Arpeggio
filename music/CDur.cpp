#include "./CDur.h"

namespace music {
    vector<Note> CDur::notes() {
        return ContinuousMusic::fromPointInTime({
            make_tuple(Pitch::C4,  0, 2000, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::D4, 2000, 2000, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::E4, 4000, 2000, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::F4, 6000, 2000, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::G4, 8000, 2000, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::A4, 10000, 2000, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::B4, 12000, 2000, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::C5, 14000, 500, make_tuple(0, 0, 1, 1)),

            make_tuple(Pitch::C5, 16000, 2000, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::B4, 18000, 2000, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::A4, 20000, 2000, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::G4, 22000, 2000, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::F4, 24000, 2000, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::E4, 26000, 2000, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::D4, 28000, 2000, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::C4, 30000, 2000, make_tuple(0, 0, 1, 1))

        });
    };
}
