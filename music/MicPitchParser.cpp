/**
 * Partially based on https://github.com/bejayoharen/guitartuner by Bjorn Roche,
 * which is available under the BSD license.
 * For explanation how pitch tracking works with FFT, please refer to his
 * excellent article at
 * http://blog.bjornroche.com/2012/07/frequency-detection-using-fft-aka-pitch.html
 **/

#include "./MicPitchParser.h"
#include "portaudio.h"
#include <vector>
#include <cmath>
#include <string.h>
#include <tuple>
#include "./libfft/libfft.h"

using namespace std;

namespace music {
    /**
     * Starts up port audio, listens on the default audio input
     **/
    MicPitchParser::MicPitchParser(int sampleRate, int fftSize, int fftExpSize) {
        this->sampleRate = sampleRate;
        this->fftSize = fftSize;
        this->fftExpSize = fftExpSize;

        window = new float[fftSize];
        freqTable = new float[fftSize];
        data = new float[fftSize];
        
        PaError err = 0;

        // build the window, fft, etc
        buildHanWindow( window, fftSize );
        fft = initfft( fftExpSize );
        computeSecondOrderLowPassParameters( sampleRate, 330, a, b );
        mem1[0] = 0; mem1[1] = 0; mem1[2] = 0; mem1[3] = 0;
        mem2[0] = 0; mem2[1] = 0; mem2[2] = 0; mem2[3] = 0;

        // freq/note tables
        for( int i=0; i<fftSize; ++i ) {
            freqTable[i] = ( sampleRate * i ) / (float) ( fftSize );
        }

        // initialize portaudio
        err = Pa_Initialize();
        if( err != paNoError ) {
            throw runtime_error( "Pa_Initialize() failed" );
        };

        inputParameters.device = Pa_GetDefaultInputDevice();
        inputParameters.channelCount = 1;
        inputParameters.sampleFormat = paFloat32;
        inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultHighInputLatency ;
        inputParameters.hostApiSpecificStreamInfo = NULL;

        printf( "Opening %s\n",
                Pa_GetDeviceInfo( inputParameters.device )->name );

        err = Pa_OpenStream( &stream,
                                &inputParameters,
                                NULL, //no output
                                sampleRate,
                                fftSize,
                                paClipOff,
                                paCallback,
                                data );
        if( err != paNoError ) {
            abort(err);
            throw runtime_error( "Pa_OpenStream failed" );
        };

        err = Pa_StartStream( stream );
        if( err != paNoError ) {
            abort(err);
            throw runtime_error( "Pa_StartStream failed" );
        };

        return;
    }

    /**
     * Stops the audio recording and performs cleanup in case of a destruction
     * (Usually at the end of the program)
     **/
    MicPitchParser::~MicPitchParser() {
        PaError err = Pa_StopStream( stream );
        if( err != paNoError ) {
            abort(err);
        };

        // cleanup
        destroyfft( fft );
        Pa_Terminate();
    }

    /**
     * Stops the audio recording and performs cleanup in case of an error
     **/
    void MicPitchParser::abort(PaError err) {
        if( stream ) {
            Pa_AbortStream( stream );
            Pa_CloseStream( stream );
        }
        destroyfft( fft );
        Pa_Terminate();
        fprintf( stderr, "An error occured while using the portaudio stream\n" );
        fprintf( stderr, "Error number: %d\n", err );
        fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
        return;
    }

    /**
     * Applies FFT to the last recorded section and finds out the note that is
     * currently being played. Analyzing the spectrum might not be the optimal
     * way to determine the note, but it has the advantage that this could be
     * modified in the future to display even more information, such as
     * overtones, and even fitting musical scales.
     **/
    vector<Note> MicPitchParser::notes() {
        float data[fftSize];
        float datai[fftSize];

        copy(this->data, this->data+fftSize, data);

        // low-pass
        for( int j=0; j<fftSize; ++j ) {
            data[j] = processSecondOrderFilter( data[j], mem1, a, b );
            data[j] = processSecondOrderFilter( data[j], mem2, a, b );
        }
        // window
        applyWindow( window, data, fftSize );

        // do the fft
        for( int j=0; j<fftSize; ++j )
            datai[j] = 0;
        applyfft( fft, data, datai, false );

        // find the peak
        float maxVal = -1;
        int maxIndex = -1;
        for( int j=0; j<fftSize/2; ++j ) {
            float v = data[j] * data[j] + datai[j] * datai[j] ;

            if( v > maxVal ) {
                maxVal = v;
                maxIndex = j;
            }
        }
        float freq = freqTable[maxIndex];

        Pitch nearestNote = nearest_pitch_from_frequency(freq);
        
        return {
            make_tuple(nearestNote, 0, 1000, make_tuple(0, 0, 0, 1))
        };
    }

    /**
     * this is the callback function that is being called by PortAudio when
     * enough data has been recorded
     **/
    int MicPitchParser::paCallback(
        const void *inputBuffer,
        void *outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,
        void *userData
    ) {
        float *input = (float *)inputBuffer;
        copy(input, input+8192, (float *)userData);

        return paContinue;
    }

    void MicPitchParser::buildHammingWindow( float *window, int size ) {
        for( int i=0; i<size; ++i )
            window[i] = .54 - .46 * cos( 2 * M_PI * i / (float) size );
    }

    void MicPitchParser::buildHanWindow( float *window, int size ) {
        for( int i=0; i<size; ++i )
            window[i] = .5 * ( 1 - cos( 2 * M_PI * i / (size-1.0) ) );
    }

    void MicPitchParser::applyWindow( float *window, float *data, int size ) {
        for( int i=0; i<size; ++i )
            data[i] *= window[i] ;
    }
    
    void MicPitchParser::computeSecondOrderLowPassParameters( float srate, float f, float *a, float *b ) {
        float a0;
        float w0 = 2 * M_PI * f/srate;
        float cosw0 = cos(w0);
        float sinw0 = sin(w0);
        //float alpha = sinw0/2;
        float alpha = sinw0/2 * sqrt(2);

        a0   = 1 + alpha;
        a[0] = (-2*cosw0) / a0;
        a[1] = (1 - alpha) / a0;
        b[0] = ((1-cosw0)/2) / a0;
        b[1] = ( 1-cosw0) / a0;
        b[2] = b[0];
    }

    float MicPitchParser::processSecondOrderFilter( float x, float *mem, float *a, float *b ) {
        float ret = b[0] * x + b[1] * mem[0] + b[2] * mem[1]
                         - a[0] * mem[2] - a[1] * mem[3] ;

		mem[1] = mem[0];
		mem[0] = x;
		mem[3] = mem[2];
		mem[2] = ret;

		return ret;
    }


}