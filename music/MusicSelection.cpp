
#include "./MusicSelection.h"
#include "./CDur.h"
#include "./CBlues.h"
#include "./Midi.h"
// uncomment the line below to include MicPitchParser. You will also need to add it to your CMake.
// #include "./MicPitchParser.h"
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    #include <stdio.h>
#else
    #include <cstdio>
#endif

using namespace std;

namespace music {
    tuple<Music*, bool> MusicSelection::selectMusic() {
        char i = 0;
        char j = 0;

        while(true) {
            system("clear");
            cout << "    BEGINNER                     ADVANCED" << endl
                 << "    --------                     --------" << endl << endl
                 << (i == 0 && j == 0 ? ">" : " ")
                 << "1. C Major                  "
                 << (i == 0 && j == 1 ? ">" : " ")
                 << "2. C Major" << endl << endl

                 << "                             "
                 << (i == 1 && j == 1 ? ">" : " ")
                 << "3. C Blues" << endl << endl

                 << "                             "
                 << (i == 2 && j == 1 ? ">" : " ")
                 << "4. Der Mond ist aufgegangen" << endl
        
                 << "                                 (MIDI)" << endl << endl

                 << "                             "
                 << (i == 3 && j == 1 ? ">" : " ")
                 << "5. Bach - Bereite dich Zion" << endl
                 << "                                 (MIDI)" << endl << endl

                 << "                             "
                 << (i == 4 && j == 1 ? ">" : " ")
                 << "6. Pitch Detection" << endl
                 << "                                 (Mic)" << endl;

            
            #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
                char c = getchar();
            #else
                system("stty raw");
                char c = getchar();
                system("stty cooked");
            #endif
            
            switch(c) {
                // On linux, ABCD cases allow to navigate using arrow keys.
                // On windows, the user shall enter the number, followed by enter key.
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
                        // uncomment the line below to include MicPitchParser
                        // m = new MicPitchParser();
                    }
                    return make_tuple(m, i != 0);
                case '1':
                    return make_tuple(new CDur(), false);
                case '2':
                    return make_tuple(new CDur(), true);
                case '3':
                    return make_tuple(new CBlues(), true);
                case '4':
                    return make_tuple(new Midi("../example_mond.mid"), true);
                case '5':
                    return make_tuple(new Midi("../example_bach.mid"), true);
                // uncomment the lines below to include MicPitchParser.
                // case '6':
                    //return make_tuple(new MicPitchParser(), true);
            }
        }

        CDur music;
        Music *m = &music;

        return make_tuple(m, false);
    };
}