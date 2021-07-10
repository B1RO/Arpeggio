#pragma once

#include "./CDur.h"

namespace music {
    vector<Note> CDur::notes() {
        return ContinuousMusic::fromPointInTime({
            make_tuple(Pitch::C4, 0, 1000, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::D4, 0, 1000, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::E4, 0, 1000, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::F4, 0, 1000, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::G4, 0, 1000, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::A4, 0, 1000, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::B4, 0, 1000, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::C5, 0, 1000, make_tuple(0, 0, 0, 1)),

            make_tuple(Pitch::C4, 1500, 500, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::D4, 2000, 500, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::E4, 2500, 500, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::F4, 3000, 500, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::G4, 3500, 500, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::A4, 4000, 500, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::B4, 4500, 500, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::C5, 5000, 500, make_tuple(0, 0, 0, 1)),

            make_tuple(Pitch::C5, 5500, 500, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::B4, 6000, 500, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::A4, 6500, 500, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::G4, 7000, 500, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::F4, 7500, 500, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::E4, 8000, 500, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::D4, 8500, 500, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::C4, 9000, 500, make_tuple(0, 0, 0, 1))
        });
    };
}
