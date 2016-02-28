//Project to control the led matrix for the Dancing Lights Project
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include <SoftwareSerial.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      64

//Set Up the NeoPixels
//Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//Delayed Value
int delayval = 500; // delay for half a second

//Length of message 
int messageLength = 8;

//Bit masks
#define bit7 0x01
#define bit6 0x02
#define bit5 0x04
#define bit4 0x08
#define bit3 0x10
#define bit2 0x20
#define bit1 0x40
#define bit0 0x80

byte masks[8] = { bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7};

//#define DEBUG

/**
 * Set up the Arduino to drive the leds
 */
void setup() {
  //pixels.begin();

  Serial.begin(9600 );

    //Draw the neo pixels
  
  while(!Serial){ ;} //Wait for serial port to connect

  Serial.setTimeout(-1); //If we have not ready bytes for 5 seconds then return;

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  
  pin(false, 2);
  pin(false, 3);
  pin(false, 4);
  pin(false, 5);
  pin(false, 6);
  pin(false, 7);
  pin(false, 8);
  pin(false, 9);
  pin(false, 10);

 /* pin(true, 2);
  pin(true, 3);
  pin(true, 4);
  pin(true, 5);
  pin(true, 6);
  pin(true, 7);
  pin(true, 8);
  pin(true, 9);
  pin(true, 10);*/


  log("Finished Setup");
}

/**
 * Main Loop
 */
void loop() {

  log("Starting Loop");
  
  byte buffer[messageLength];

  int error = 0;
  Serial.readBytes(buffer, messageLength);

  log("Returned from reading");
  
  if( buffer[0] != ':'){
      log("Detected an error");
      
    //Now we need to correct for a lost byte somewhere
    for( int i = 0; i <= messageLength; i++){
      if( i == messageLength){
        return; //Just start over
      }
      if( buffer[i] == ':'){
      //We are offset by i
        for( int j = 0; j < (messageLength - i); j++){
          buffer[j] = buffer[i+j];
        }
        byte recov[messageLength];
        Serial.readBytes(recov, messageLength-i);
        for( int j = 0; j< (messageLength-i); j++){
          buffer[i+j] = recov[j];
        }
        break; //By now we should have recovered and now we need to continue flashing lights
      }  
    }
  }
  else{
    error = 0;
  }
 
  //Buffer should contain the latest notes here
  
  log("Drawing lights");
  
  //Light 2 - BASS
  bool twoDisp = false;
  for( int i = 0; i < 4 ; i++){
    twoDisp = twoDisp || shouldDisplay(buffer, i);
  }
  pin(twoDisp, 2);

  //Other Lights
  for( int i =3, j = 10; i < 11; i++, j+=3){
    //For each Light check its range
      if( shouldDisplay(buffer, j+0) || shouldDisplay(buffer, j+1) || shouldDisplay(buffer, j+2)){
        pin(true, i);
      }
      else{
        pin(false, i);
      }
  }
}

/**
 * Return true or false for a given bit being flipped
 */
bool shouldDisplay(byte* buffer, int index){
  int convertedIndex = index + 8; 

  int octave = convertedIndex /8;
  int ind = convertedIndex %8;
  

  if( (buffer[octave] & masks[ind]) != 0) { 
    return true;
  }
  return false;
}

/**
 * 
 */
void log(const char* logger){
  #ifdef DEBUG
  Serial.println(logger);
  #endif
}

/**
 * Turn on a pin
 */

void pin(bool on, int pin){
  if( on){
    digitalWrite(pin, HIGH);
  }
  else{
    digitalWrite(pin, LOW);
  }
}

