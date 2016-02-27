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
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

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

#define DEBUG

/**
 * Set up the Arduino to drive the leds
 */
void setup() {
  pixels.begin();

  Serial.begin(115200);

    //Draw the neo pixels
  for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    //pixels.setPixelColor(i, pixels.Color(0,255,0)); // Moderately bright green color.

    pixels.setPixelColor(i, pixels.Color(255,0,0));
    
    pixels.show(); // This sends the updated pixel color to the hardware.
  }
  
  while(!Serial){ ;} //Wait for serial port to connect

  Serial.setTimeout(-1); //If we have not ready bytes for 5 seconds then return;

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

  //Draw the neo pixels
  for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    //pixels.setPixelColor(i, pixels.Color(0,255,0)); // Moderately bright green color.

    pixels.setPixelColor(i, pixels.Color(0,0,255));
    
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(delayval); // Delay for a period of time (in milliseconds).

  }

}

/**
 * Return true or false for a given bit being flipped
 */
bool shouldDisplay(byte* buffer, int index){
  int convertedIndex = index -4; 

  int octave = convertedIndex /8;

  if( (buffer[octave] & ((byte)(convertedIndex % 8))) != 0) {
    return true;
  }
  return false;
}

/**
 * 
 */
void log(char * logger){
  #ifdef DEBUG
  Serial.println(logger);
  #endif
}

