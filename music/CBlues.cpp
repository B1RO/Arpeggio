#pragma once

#include "./CBlues.h"

namespace music {
    vector<Note> CBlues::notes() {
        return ContinuousMusic::fromPointInTime({
            make_tuple(Pitch::C4,     0, 1000, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::Eflat4, 0, 1000, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::F4,     0, 1000, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::Gflat4, 0, 1000, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::G4,     0, 1000, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::Bflat4, 0, 1000, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::C5,     0, 1000, make_tuple(0, 0, 0, 1)),

            make_tuple(Pitch::C4,     1500, 500, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::Eflat4, 2000, 500, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::F4,     2500, 500, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::Gflat4, 3000, 500, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::G4,     3500, 500, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::Bflat4, 4000, 500, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::C5,     4500, 500, make_tuple(0, 0, 0, 1)),

            make_tuple(Pitch::Bflat4, 5000, 500, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::G4,     5500, 500, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::Gflat4, 6000, 500, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::F4,     6500, 500, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::Eflat4, 7000, 500, make_tuple(0, 0, 0, 1)),
            make_tuple(Pitch::C4,     7500, 500, make_tuple(0, 0, 0, 1))
        });
    };
}