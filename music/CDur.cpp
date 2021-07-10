#include "./CDur.h"

namespace music {
    vector<Note> CDur::notes() {
        return ContinuousMusic::fromPointInTime({
            make_tuple(Pitch::C4,  500, 500, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::D4, 1000, 500, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::E4, 1500, 500, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::F4, 2000, 500, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::G4, 2500, 500, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::A4, 3000, 500, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::B4, 3500, 500, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::C5, 4000, 500, make_tuple(0, 0, 1, 1)),

            make_tuple(Pitch::C5, 4500, 500, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::B4, 5000, 500, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::A4, 5500, 500, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::G4, 6000, 500, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::F4, 6500, 500, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::E4, 7000, 500, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::D4, 7500, 500, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::C4, 8000, 500, make_tuple(0, 0, 1, 1))
        });
    };
}
