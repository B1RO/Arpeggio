#include <iostream>
#include <vector>
#include "Pitch.h"

using namespace std;

namespace music {

    /**
   * Pitch in Hz
   */
    typedef double frequency;

    /**
     * Converts Midi pitch value to frequency in Hz
     * Conversion taken from:
     * https://www.inspiredacoustics.com/en/MIDI_note_numbers_and_center_frequencies
     */
    frequency frequency_from_pitch(Pitch p, frequency baseFrequency = 440.0f) {
        return baseFrequency * (2 ^ (((int) p - 69) / 12));
    }

    Pitch nearest_pitch_from_frequency(frequency f, frequency baseFrequency = 440.0f) {
        return (Pitch) (f / baseFrequency); // TODO finish
    }

    /**
     * A note is a tuple of
     * - a Midi note value,
     * - a time in ms in which it will be played from wrapping class initialization,
     * - a length of the note in ms,
     * - and a RGBA color that it shall be represented in.
     */
    typedef tuple<Pitch, int, int, tuple<float, float, float, float>> Note;


    /**
     * Objects of Music classes provide notes, eg for displaying on a keyboard
     */
    class Music {
    public:
        /**
         * Returns a vector of all notes that will at some point be played.
         * Depending on the implementation, the return value might vary
         * depending on realtime events, so this should be called regularly
         * to update.
         */
        virtual vector<Note> notes() = 0;
    };
}
