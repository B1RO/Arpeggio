#include "./Midi.h"
#include "./midifile/MidiFile.h"

using namespace std;
using namespace smf;

namespace music {

    Midi::Midi(string file) {
        importedNotes = {};

        // Usually, one or two colors should be seen (depending on if the user plays with
        // one or two hands at once). There might be scenarios where more than two hands
        // play, thus there are four colors. After that, the colors repeat.
        tuple<float, float, float, float> trackColors[] = {
            make_tuple(1.0f, 0.0f, 0.0f, 1.0f),
            make_tuple(0.0f, 1.0f, 0.0f, 1.0f),
            make_tuple(0.0f, 0.0f, 1.0f, 1.0f),
            make_tuple(1.0f, 1.0f, 1.0f, 1.0f)
        };

        MidiFile midifile;
        midifile.read(file);
        midifile.doTimeAnalysis();
        midifile.linkNotePairs();

        // the following part is partially based on the documentation for midifile at
        // https://github.com/craigsapp/midifile#midi-file-reading-examples
        int tracks = midifile.getTrackCount();
        for (int track=0; track<tracks; track++) {
            for (int event=0; event<midifile[track].size(); event++) {

                auto n = midifile[track][event];
                for (int i=0; i<midifile[track][event].size(); i++)
                    cout << (int)midifile[track][event][i] << ' ';
                cout << endl;
                // if event starts note
                if((int)n[0] == 0x90 && (int)n[2] == 0x50) {
                    Pitch pitch = (Pitch)midifile[track][event].getP1();
                    int time = midifile[track][event].seconds * 1000;
                    int duration = (int)(midifile[track][event].getDurationInSeconds() * 1000);

                    // cout << (int)pitch << " " << time << " " << duration << endl;
                    importedNotes.push_back(make_tuple(pitch, time, duration, trackColors[track % 4]));
                }
            }
        }
    }

    vector<Note> Midi::notes() {
        return ContinuousMusic::fromPointInTime(importedNotes);
    };

}