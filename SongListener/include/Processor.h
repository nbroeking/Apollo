/* This is the processor that returns a char array for instructing the arduino 
Written By: Nicolas Broeking */

#ifndef __LIGHTPROCESSOR__
#define __LIGHTPROCESSOR__

#include <fftw3.h>

class Processor
{
public:
   	Processor();
	virtual ~Processor();

    virtual void process(fftw_complex array[513], int size, int8_t* result, int integral);

    const int numNotes = 51;

protected:

    bool isMax(fftw_complex  buffer[513], int index );
    bool isClose(fftw_complex  buffer[513], int index, int compare);
    void setVal(int8_t* message, int index, int val);
    double threshold(int i);
    uint8_t bit[8];
    const uint8_t bit7 = 0x01;
    const uint8_t bit6 = 0x02;
    const uint8_t bit5 = 0x04;
    const uint8_t bit4 = 0x08;
    const uint8_t bit3 = 0x10;
    const uint8_t bit2 = 0x20;
    const uint8_t bit1 = 0x40;
    const uint8_t bit0 = 0x80;
};
#endif
