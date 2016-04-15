#include "ScopedLock.h"
#include "Processor.h"
#include <cstring>
#include <math.h>
#include <unistd.h>
#include <iostream>
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
    return 0;

        //return -0.0555*i + 50;
}

void Processor::setVal(int8_t* message, int index, int val){
    int convertedIndex = index + 8; 

    int octave = convertedIndex /8;
    int ind = (convertedIndex) %8;
    
    if( val){
        message[octave] = message[octave] | bit[ind];
    }
}

bool Processor::isMax(fftw_complex buffer[513], int index, double integral ){
    
    if( integral == 0){
        return false;
    }
    double ourValue = buffer[index][0]/ integral;
    double less = buffer[index-1][0]/integral;
    double more = buffer[index+1][0]/integral;

    if( (ourValue >= less) && (ourValue >= more)){
        return true;
    }
    return false;
}

bool Processor::isClose(fftw_complex  buffer[513], int index, int compare){
    double val1 = buffer[index][0];
    double val2 = buffer[compare][0];

    if( val1 > 0.8*val2){
        return true;
    }
    return false;
}

void Processor::process(fftw_complex array[513], int size, int8_t* result, int integral){
    //Start setting up for the result
    result[0] = ':';

    for( int i = 1; i < 8; i++){
        result[i] = 0;
    }

    if( integral > 0 && array[0][0] > array[1][0] && (array[0][0] > (integral/7))){
        setVal(result, 0, 1);        
    }
   

    for (int i = 2; i < (size-1); ++i)
    {
        if( isMax(array, i, (double)integral) && (array[i][0] > (integral/8)))//|| (isClose(array, i, i+1) && isMax(array, i+1)) || (isClose(array, i, i-1) && isMax(array,i+1))){
        {    
            setVal(result, i, 1);
        }
    }
}
