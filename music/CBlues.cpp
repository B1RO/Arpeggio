#include "./CBlues.h"

namespace music {
    vector<Note> CBlues::notes() {
        return ContinuousMusic::fromPointInTime({
            make_tuple(Pitch::C4,      500, 500, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::Eflat4, 1000, 500, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::F4,     1500, 500, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::Gflat4, 2000, 500, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::G4,     2500, 500, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::Bflat4, 3000, 500, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::C5,     3500, 500, make_tuple(0, 0, 1, 1)),

            make_tuple(Pitch::Bflat4, 4000, 500, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::G4,     4500, 500, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::Gflat4, 5000, 500, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::F4,     5500, 500, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::Eflat4, 6000, 500, make_tuple(0, 0, 1, 1)),
            make_tuple(Pitch::C4,     6500, 500, make_tuple(0, 0, 1, 1))
        });
    };
}