#include "ScopedLock.h"
#include "Processor.h"
#include <cstring>
#include <math.h>
#include <unistd.h>

using namespace std;



//Constructor
Processor::Processor()
{
    bit[0] = bit0;
    bit[1] = bit1;
    bit[2] = bit2;
    bit[3] = bit3;
    bit[4] = bit4;
    bit[5] = bit5;
    bit[6] = bit6;
    bit[7] = bit7;
}



Processor::~Processor()
{}

double Processor::threshold(int i){
    return 55;
    //return -0.0555*i + 50;
}
void Processor::process(fftw_complex array[513], int size, int8_t* result){
    //Start setting up for the result
    result[0] = ':';

    for( int i = 1; i < 8; i++){
        result[i] = 0;
    }

    int octCount = 1;
    int offset = 0;

    for (int i = 0; i < size; ++i)
    {
        double currentVal = array[i][0];

        if( currentVal > threshold(i)){
            result[octCount] = result[octCount] | bit[(i+offset)%8];
        }

        //Moving to next bitmap
        if( i%6 == 0){
            offset += 1;
            octCount +=1;
        }        
    }
}
