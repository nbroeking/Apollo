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

#define BUFSIZE 4096
#define INSIZE (BUFSIZE/2)
#define OUTPUTSIZE ((INSIZE/2)+1)
#define MAXFREQ 22050
#define MAXANALYZE (OUTPUTSIZE/4)

class MainApplication
{
public:
   	MainApplication();
	virtual ~MainApplication();

    virtual int run();
	virtual void pleaseDie();

protected:

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


	std::mutex m; //Access Mutex
	BlockingQueue q;
	std::mutex runMutex; // So only one thread can run the application at a time
};
#endif
