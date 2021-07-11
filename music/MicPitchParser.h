#include "./Music.h"
//#include "portaudio.h"
#include <chrono>

using namespace std;
using namespace music;

namespace music {
    /**
     * Provides the note that is currently recognized via microphone input. Possible use cases:
     * - User sings notes or melody to find out how to play it on piano
     * - Can help with orientation on keyboard in scenarios where call and response patterns are
     *   used (another musician plays something, user responds with something similar)
     * The algorithm uses Fast Fourier Transform, so we get a lot more audio information than
     * just the pitch - I plan to add a class that is based on this one that shows the user
     * overtones or even fitting scales. I'd like to see how good this works first though.
     **/
    class MicPitchParser : public Music {
    public:
        /**
         * For now, please refrain to use different values than the defaults.
         **/
        MicPitchParser(int sampleRate = 8000, int fftSize = 8192, int fftExpSize = 13);
        ~MicPitchParser();
        /**
         * Always returns one note, the one that is currently recognized via microphone
         */
        vector<Note> notes();
    private:
        static int paCallback(
            const void *inputBuffer,
            void *outputBuffer,
            unsigned long framesPerBuffer,
            const PaStreamCallbackTimeInfo* timeInfo,
            PaStreamCallbackFlags statusFlags,
            void *userData );
        void buildHammingWindow( float *window, int size );
        void buildHanWindow( float *window, int size );
        void applyWindow( float *window, float *data, int size );
        //a must be of length 2, and b must be of length 3
        void computeSecondOrderLowPassParameters( float srate, float f, float *a, float *b );
        //mem must be of length 4.
        float processSecondOrderFilter( float x, float *mem, float *a, float *b );
        float *data;
        int sampleRate;
        int fftSize;
        int fftExpSize;
        PaStreamParameters inputParameters;
        float a[2], b[3], mem1[4], mem2[4];
        float *window, *freqTable;
        void * fft = NULL;
        PaStream *stream = NULL;
        void abort(PaError e);
    };
}