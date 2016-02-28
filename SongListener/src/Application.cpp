#include "ScopedLock.h"
#include "Event.h"
#include "Log.h"
#include "Application.h"
#include <fftw3.h>
#include <cstring>
#include <math.h>
#include <unistd.h>
#include "Processor.h"
using namespace std;

//Main Running Loop
int MainApplication::run()
{
	//No two threads can try and run the application at a time. It shouldn't
	// happen anyway but you never know what mistakes i will make.
	if( !runMutex.try_lock() || isRunning == true)
	{
		return 1;
	}
	m.lock();
	isRunning = true;
	
    //Initilization
	shouldRun.store(true);

    if (!(s = pa_simple_new(NULL, "Song Listener", PA_STREAM_RECORD, NULL, "record", &ss, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
        return 2;
    }
    m.unlock();
	while (shouldRun.load())
	{
	    int16_t buf[BUFSIZE];
       
        #ifndef DEBUG 
        /* Record some data ... */
        if (pa_simple_read(s, buf, sizeof(buf), &error) < 0) {
            fprintf(stderr, __FILE__": pa_simple_read() failed: %s\n", pa_strerror(error));
            return 3;
        }
        #else


        #endif
        //THis is a bad check so we only do the fft on real data
        if( buf[0] != 0){
           
            //Combining the left and right streams
           for( int i = 0; i < INSIZE; i++){
                int16_t left = buf[i*2];
                int16_t right = buf[i*2 +1];

                in[i] = ((float)left + (float)right) / 65536.f;
            }

            //Windowing I think 
            for( int i =0; i < INSIZE; i++){
                //in[i] = buf[i];
                in[i] *= 0.54f - 0.46f * cosf( (3.1415926 * 2.f * i) / (BUFSIZE - 1) ); 
            }


            fftw_execute(p);

            // I rewrite to out[i][0] squared absolute value of a complex number out[i].
            for (int i = 0; i < MAXANALYZE; ++i)
            {
                out[i][0] = 10*log(out[i][0]*out[i][0] + out[i][1]*out[i][1]);
            }
   
           
            int8_t  bitmask[8];

            processor.process(out, MAXANALYZE, bitmask);

            arduino << bitmask;
            arduino.flush();

            /*int start = 0.0;
            for( int i = 0; i < MAXANALYZE; i++){
                cerr << start << ", " <<  out[i][0] << endl;
                start += MAXFREQ/OUTPUTSIZE;

            }*/
            //exit(0);
        }
    }

	m.lock();
	isRunning = false;
	m.unlock();

	runMutex.unlock();

    printf("Application Exiting Normally\n");
	return 0;
}

//Constructor
MainApplication::MainApplication():
//bass("5"),
//low("26"),
//med("24"),
//high("19"),
//highest("6"),
m(),
q(),
runMutex()
{
	isRunning = false;
	shouldRun.store(true);

    arduino.open("/dev/ttyACM0");
    s = NULL;
   
   for (int i = 0; i < BUFSIZE; ++i){
        in[i] = 0.f;
    }
   for (int i = 0; i < OUTPUTSIZE; ++i){
        out[i][0] = 0.0;
        out[i][1] = 0.0;
    }
    p = fftw_plan_dft_r2c_1d(INSIZE, in, out, FFTW_ESTIMATE); 
}


MainApplication::~MainApplication()
{
    printf("Shutting down the Main Application\n");
    arduino.close();
    if( s != NULL){
        pa_simple_free(s);
        printf("Successfully cleaned up the pulse audio library\n");
    }
    else{
        printf("Application could not clean up pulse objects\n");
    }
    fftw_destroy_plan(p);
    fftw_cleanup();
}
void MainApplication::pleaseDie()
{
	printf("Asking the Main application to die\n");
    //Add a close event to the application
    shouldRun.store(false);
}
