#include "ScopedLock.h"
#include "Event.h"
#include "Log.h"
#include "Application.h"
#include <fftw3.h>
#include <cstring>
#include <math.h>
#include <unistd.h>

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
	while (shouldRun)
	{
	    int8_t buf[BUFSIZE];
       
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
           
            cout<< "Start of raw data LRLRLRLR... of 2048 values\n";
            for (int i = 0; i < BUFSIZE; i++){
                printf("%d, ", buf[i]);
            }
            cout << endl << "End of raw data\n";

            //Combining the left and right streams
           /* for( int i = 0; i < INSIZE; i++){
                int16_t left = buf[i*2];
                int16_t right = buf[i*2 +1];

                in[i] = ((float)left + (float)right) / 65536.f;
            }*/

            for( int i = 0; i < INSIZE; i++){
                int16_t left = buf[i*2];
                //int16_t right = buf[i*2 +1];
                //in[i] = (float) buf[i];
                in[i] = ((float)left) / 65536.f;
            }
          
            cout<< "Start of combined data\n";
            for (int i = 0; i < INSIZE; i++){
                printf("%f, ", in[i]);
            }
            cout << endl << "End of raw data\n";


            //Windowing I think 
/*            for( int i =0; i < INSIZE; i++){
                //in[i] = buf[i];
                in[i] *= 0.54f - 0.46f * cosf( (3.1415926 * 2.f * i) / (BUFSIZE - 1) ); 
            }
*/
            float start = 0.0;

            fftw_execute(p);

            // I rewrite to out[i][0] squared absolute value of a complex number out[i].
            for (int i = 0; i < MAXANALYZE; ++i)
            {
                out[i][0] = out[i][0]*out[i][0] + out[i][1]*out[i][1];
            }
            
            for( int i = 0; i < MAXANALYZE; i++){
                cerr << start << ", " <<  out[i][0] << endl;
                start += MAXFREQ/INSIZE;

            }
            exit(0);          
        }
        printf("Completed sample %d\n", buf[0]);   
        //Do a FFT 
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
m(),
q(),
runMutex()
{
	isRunning = false;
	shouldRun.store(true);

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
    if( s != NULL){
        pa_simple_free(s);
    }
    else{
        printf("Application could not clean up pulse objects\n");
    }
    fftw_destroy_plan(p);
}
void MainApplication::pleaseDie()
{
//	SystemLog("Please Die in Main Application");
	//Add a close event to the application
    shouldRun.store(false);
}
