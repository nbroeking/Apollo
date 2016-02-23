//This is the main application
// It returns the result of the application
// Created by Nicolas Broeking
// Created on: July 23, 2014

/* SDL Initilization here just to be sure it gets done promptly 

*/

//The includes

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pulse/simple.h>
#include <pulse/error.h>
#include <fftw3.h>
#include "Application.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

MainApplication *globalapp;

extern "C" {
    void timeToDie(int s);
};

int main(int argc, char*argv[]) {

    MainApplication app;
    globalapp = &app;
   
    signal (SIGINT, timeToDie);

    return app.run();
}

void timeToDie(int s){
    printf("Caught signal %d\n",s);

    globalapp->pleaseDie();
}
