#include <tuple>
#include <GL/gl.h>

using namespace std;

namespace music {

    /**
     * Midi pitch values values as described here:
     * https://www.inspiredacoustics.com/en/MIDI_note_numbers_and_center_frequencies
     * 
     * On a keyboard:
     * +--+----+-+----+--+--+----+-+----+-+----+--+
     * |  |    | |    |  |  |    | |    | |    |  |
     * |  |    | |    |  |  |    | |    | |    |  |
     * |  | C# | | D# |  |  | F# | | G# | | A# |  |
     * |  | Db | | Eb |  |  | Gb | | Ab | | Bb |  |
     * |  |    | |    |  |  |    | |    | |    |  |
     * |  |    | |    |  |  |    | |    | |    |  |
     * |  +----+ +----+  |  +----+ +----+ +----+  |
     * |     |     |     |     |      |     |     |
     * |     |     |     |     |      |     |     |
     * |  C  |  D  |  E  |  F  |   G  |  A  |  B  |
     * |     |     |     |     |      |     |     |
     * |     |     |     |     |      |     |     |
     * +-----+-----+-----+-----+------+-----+-----+
     * 
     * In this enum, "sharp" means "#" and "flat" means "b".
     */
    enum class Pitch {
        A0      = 21,
        Asharp0 = 22,
        Bflat0  = 22,
        B0      = 23,

        C1      = 24,
        Csharp1 = 25,
        Dflat1  = 25,
        D1      = 26,
        Dsharp1 = 27,
        Eflat1  = 27,
        E1      = 28,
        F1      = 29,
        Fsharp1 = 30,
        Gflat1  = 30,
        G1      = 31,
        Gsharp1 = 32,
        Aflat1  = 32,
        A1      = 33,
        Asharp1 = 34,
        Bflat1  = 34,
        B1      = 35,

        C2      = 36,
        Csharp2 = 37,
        Dflat2  = 37,
        D2      = 38,
        Dsharp2 = 39,
        Eflat2  = 39,
        E2      = 40,
        F2      = 41,
        Fsharp2 = 42,
        Gflat2  = 42,
        G2      = 43,
        Gsharp2 = 44,
        Aflat2  = 44,
        A2      = 45,
        Asharp2 = 46,
        Bflat2  = 46,
        B2      = 47,

        C3      = 48,
        Csharp3 = 49,
        Dflat3  = 49,
        D3      = 50,
        Dsharp3 = 51,
        Eflat3  = 51,
        E3      = 52,
        F3      = 53,
        Fsharp3 = 54,
        Gflat3  = 54,
        G3      = 55,
        Gsharp3 = 56,
        Aflat3  = 56,
        A3      = 57,
        Asharp3 = 58,
        Bflat3  = 58,
        B3      = 59,

        C4      = 60,
        Csharp4 = 61,
        Dflat4  = 61,
        D4      = 62,
        Dsharp4 = 63,
        Eflat4  = 63,
        E4      = 64,
        F4      = 65,
        Fsharp4 = 66,
        Gflat4  = 66,
        G4      = 67,
        Gsharp4 = 68,
        Aflat4  = 68,
        A4      = 69,
        Asharp4 = 70,
        Bflat4  = 70,
        B4      = 71,

        C5      = 72,
        Csharp5 = 73,
        Dflat5  = 73,
        D5      = 74,
        Dsharp5 = 75,
        Eflat5  = 75,
        E5      = 76,
        F5      = 77,
        Fsharp5 = 78,
        Gflat5  = 78,
        G5      = 79,
        Gsharp5 = 80,
        Aflat5  = 80,
        A5      = 81,
        Asharp5 = 82,
        Bflat5  = 82,
        B5      = 83,

        C6      = 84,
        Csharp6 = 85,
        Dflat6  = 85,
        D6      = 86,
        Dsharp6 = 87,
        Eflat6  = 87,
        E6      = 88,
        F6      = 89,
        Fsharp6 = 90,
        Gflat6  = 90,
        G6      = 91,
        Gsharp6 = 92,
        Aflat6  = 92,
        A6      = 93,
        Asharp6 = 94,
        Bflat6  = 94,
        B6      = 95,

        C7      = 96,
        Csharp7 = 97,
        Dflat7  = 97,
        D7      = 98,
        Dsharp7 = 99,
        Eflat7  = 99,
        E7      = 100,
        F7      = 101,
        Fsharp7 = 102,
        Gflat7  = 102,
        G7      = 103,
        Gsharp7 = 104,
        Aflat7  = 104,
        A7      = 105,
        Asharp7 = 106,
        Bflat7  = 106,
        B7      = 107,

        C8      = 108
    };

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
}