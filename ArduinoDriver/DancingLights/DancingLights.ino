///Project to control the led matrix for the Dancing Lights Project
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include <SoftwareSerial.h>
//#include <Random.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            3

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      64
#define ROW 8
#define INNERROW (ROW-2)

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

byte masks[8] = { bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7};

//#define DEBUG

//Bass iteration
int bassIter = 0;

/**
 * Set up the Arduino to drive the leds
 */
void setup() {

  Serial.begin(9600 );

  randomSeed(0);
  
    //Draw the neo pixels
  pixels.begin();

  
  drawBass(0,0,0);
  for( int k = 0; k < 36; ++k){
    draw(-1, k);

  }
  pixels.show();
  
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

  #ifdef DEBUG
  for( int i = 1; i< 8; i++){
    Serial.println(buffer[i], BIN);
  }

  #endif
  //Buffer should contain the latest notes here
  
  log("Drawing lights");
  
  //Light 2 - BASS
  bool twoDisp = false;
/*  Commented out the detection for drawing the bass border. Bass is present in every song and displaying the bass
    made it so that you could not see the other lights up. Instead of overwealming the watcher we are now just going to
    ignore the bass. 
  for( int i = 0; i < 3 ; i++){
    twoDisp = twoDisp || shouldDisplay(buffer, i);
  }
*/
  if( twoDisp){
  
    int c = random(1, 4);
    int x,y,z;
    x = y = z = 0;

    int color = random(255);

    if( c == 1){
      drawBass(color, 255 - color, 0);
    }
    else if( c == 2){
      drawBass(color, 0, 255- color);
    }
    else{
      drawBass(0, color, 255-color);
    }

  }
  else{
    drawBass(0,0,0);
  }
  
  for( int i = 0; i < INNERROW*INNERROW; i++){
    if( shouldDisplay(buffer, i+3)){
      draw(i, i);
    }
    else{
      draw(-1, i);
    }
  }
  
  pixels.show();
}

/**
 * Return true or false for a given bit being flipped
 */
bool shouldDisplay(byte* buffer, int index){
  int convertedIndex = index + 8; 

 
  //int octave = convertedIndex /7;
  //int ind = (convertedIndex+octave) %8;

  int octave = convertedIndex /8;
  int ind = (convertedIndex) %8;

  if( (buffer[octave] & masks[ind]) != 0) { 
    return true;
  }
  return false;
}

void draw(int pos, int n){
  uint32_t color;

  int realIndex = n+9+2*(n/INNERROW);
  
  if( pos < 0){
    pixels.setPixelColor(realIndex, pixels.Color(0,0,0));
  }
  else{
    pixels.setPixelColor(realIndex, Wheel((((pos * 256) / 36))));
  }
}

void drawBass(uint32_t red, uint32_t green, uint32_t blue){
  
  for( int i = 0; i < ROW; i++){
    if( (i+1+bassIter)%3 == 0){
      pixels.setPixelColor(i, pixels.Color(red, green, blue));
    }
    if( (i+1+bassIter)%3 == 0){
      pixels.setPixelColor(ROW*7 + i, pixels.Color(red, green, blue));
    }
    if( (i+bassIter)%3 == 0){
      pixels.setPixelColor(ROW*i , pixels.Color(red, green, blue));
    }
    if( (i+bassIter)%3 == 0){
      pixels.setPixelColor(ROW*i + 7 , pixels.Color(red, green, blue));
    }
  }
  bassIter += 1;

  bassIter = bassIter%3;
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

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

