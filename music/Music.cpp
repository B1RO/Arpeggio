#include "./Music.h"
#pragma once
namespace music {
    frequency frequency_from_pitch(Pitch p, frequency baseFrequency) {
        return baseFrequency * (2 ^ (((int) p - 69) / 12));
    }

    Pitch nearest_pitch_from_frequency(frequency f, frequency baseFrequency) {
        return (Pitch) fmax(
            21,
            fmin(
                round(log2(f / baseFrequency) * 12 + 69),
                108
            )
        );
    }

    bool isBlackKey(Pitch p) {
        int k = (int)p % 12;
        if(k == 1 || k == 3 || k == 6 || k == 8 || k == 10) {
            return true;
        } else {
            return false;
        }
    }
}