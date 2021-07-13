
#include "./MusicSelection.h"
#include "./CDur.h"
#include "./CBlues.h"
#include "./Midi.h"
//#include "./MicPitchParser.h"
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
            cout << "    BEGINNER                     INTERMEDIATE" << endl
                 << "    --------                     ------------" << endl << endl
                 << (i == 0 && j == 0 ? ">" : " ")
                 << "1. C Major                  "
                 << (i == 0 && j == 1 ? ">" : " ")
                 << "2. C Major" << endl << endl

                 << (i == 1 && j == 0 ? ">" : " ")
                 << "3. C Blues                  "
                 << (i == 1 && j == 1 ? ">" : " ")
                 << "4. C Blues" << endl << endl

                 << (i == 2 && j == 0 ? ">" : " ")
                 << "5. Der Mond ist aufgegangen "
                 << (i == 2 && j == 1 ? ">" : " ")
                 << "6. Der Mond ist aufgegangen" << endl
        
                 << "    (MIDI)                       (MIDI)" << endl << endl

                 << "                             "
                 << (i == 3 && j == 1 ? ">" : " ")
                 << "7. Bach - Bereite dich Zion" << endl

                 << "                                 (MIDI)" << endl << endl

                 << "                             "
                 << (i == 4 && j == 1 ? ">" : " ")
                 << "8. Pitch Detection" << endl
                 << "                                 (Mic)" << endl;

            
            #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
                char c = getchar();
            #else
                system("stty raw");
                char c = getchar();
                system("stty cooked");
            #endif
            
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
                      //  m = new MicPitchParser();
                    }
                    return make_tuple(m, i == 0 ? false : true);
                case '1':
                    return make_tuple(new CDur(), false);
                case '2':
                    return make_tuple(new CDur(), true);
                case '3':
                    return make_tuple(new CBlues(), false);
                case '4':
                    return make_tuple(new CBlues(), true);
                case '5':
                    return make_tuple(new Midi("../example_mond.mid"), false);
                case '6':
                    return make_tuple(new Midi("../example_mond.mid"), true);
                case '7':
                    return make_tuple(new Midi("../example_bach.mid"), true);
                //case '8':
                    //return make_tuple(new MicPitchParser(), true);
            }
        }

        CDur music;
        Music *m = &music;

        return make_tuple(m, false);
    };
}