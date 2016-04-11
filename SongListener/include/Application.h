/* This is the main application. 
Pattern: Singleton
File: MainApplicaion.mm
Written By: Nicolas Broeking */

#ifndef __MainApplication__
#define __MainApplication__

#include "EventQueue.h"
#include <mutex>
#include <atomic>
#include <errno.h>
#include <pulse/simple.h>
#include <pulse/error.h>
#include <fftw3.h>
#include "Pin.h"
#include "Processor.h"
#include <fstream>
#include <iostream>

//#define BUFSIZE 2048
#define BUFSIZE 4096
#define INSIZE (BUFSIZE/2)
#define OUTPUTSIZE ((INSIZE/2)+1)
#define MAXFREQ 44100/2
#define STEPSIZE MAXFREQ/OUTPUTSIZE
#define MAXANALYZE 56 //For each bit
#define THRESHOLD 25

class MainApplication
{
public:
   	MainApplication();
	virtual ~MainApplication();

    virtual int run();
	virtual void pleaseDie();

protected:

    Processor processor;

    //FFT Variables
    fftw_plan p;
    double in[INSIZE];
    fftw_complex out[OUTPUTSIZE];

    bool isRunning;
    std::atomic<bool> shouldRun;

    const pa_sample_spec ss = {
        .format = PA_SAMPLE_S16LE,
        .rate = 44100,
        .channels = 2
    };
    pa_simple *s;
    int error; 

    FILE*  arduino;
    //Pin bass;
    //Pin low;
    //Pin med;
    //Pin high;
    //Pin highest;
};
#endif
