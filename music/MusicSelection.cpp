
#include "./MusicSelection.h"
#include "./CDur.h"
#include "./CBlues.h"
#include "./Midi.h"
#include "./MicPitchParser.h"
#include <cstdio>

using namespace std;

namespace music {
    tuple<Music*, bool> MusicSelection::selectMusic() {
        char i = 0;
        char j = 0;

        while(true) {
            system("clear");
            cout << "  BEGINNER                 INTERMEDIATE" << endl
                 << "  --------                 ------------" << endl << endl
                 << (i == 0 && j == 0 ? ">" : " ")
                 << " C Major                  "
                 << (i == 0 && j == 1 ? ">" : " ")
                 << " C Major" << endl << endl

                 << (i == 1 && j == 0 ? ">" : " ")
                 << " C Blues                  "
                 << (i == 1 && j == 1 ? ">" : " ")
                 << " C Blues" << endl << endl

                 << (i == 2 && j == 0 ? ">" : " ")
                 << " Der Mond ist aufgegangen "
                 << (i == 2 && j == 1 ? ">" : " ")
                 << " Der Mond ist aufgegangen" << endl
        
                 << "  (MIDI)                     (MIDI)" << endl << endl

                 << "                           "
                 << (i == 3 && j == 1 ? ">" : " ")
                 << " Bach - Bereite dich Zion" << endl

                 << "                             (MIDI)" << endl << endl

                 << "                           "
                 << (i == 4 && j == 1 ? ">" : " ")
                 << " Pitch Detection" << endl
                 << "                             (Mic)" << endl;

            
            system("stty raw");
            char c = getchar();
            system("stty cooked");
            
            switch(c) {
                case 'A':
                    i = (i-1)%8;
                    break;
                case 'D':
                    j = (j+1)%2;
                    break;
                case 'B':
                    i = (i+1)%8;
                    break;
                case 'C':
                    j = (j+1)%2;
                    break;
                case '\n':
                case '\r':
                    Music *m;
                    if(i == 0) {
                        m = new CDur();
                    } else if(i == 1) {
                        m = new CBlues();
                    } else if(i == 2) {
                        m = new Midi("../example_mond.mid");
                    } else if(i == 3) {
                        m = new Midi("../example_bach.mid");
                    } else if(i == 4) {
                        m = new MicPitchParser();
                    }
                    return make_tuple(m, i == 0 ? false : true);
            }
        }

        CDur music;
        Music *m = &music;

        return make_tuple(m, false);
    };
}