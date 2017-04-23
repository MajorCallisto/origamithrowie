/* Light Programmer
 * Animates a transition between rgb values
 * Christopher Lewis
 * Started: March 2017
 * 
 */
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN            1 //attiny85 physical pin 6 maps to pin 1 in software
#define NUMPIXELS      1

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//Class to store color information.
class OrigamiCubeColor{
  public: int r = 0;
  public: int g = 0;
  public: int b = 0;
  public: unsigned long transitionTime = 0;//store the amount of time between colors
  public: uint32_t color = 0;
  public: String colorStatus = "";
  
  public:
  OrigamiCubeColor(int _r, int _g, int _b, unsigned long _transitionTime){
    r =_r;
    g=_g;
    b=_b;
    transitionTime = _transitionTime;

    color = ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
  }
};
//PASTE HERE
#define NUMCOLORS 3
OrigamiCubeColor cubeColors[NUMCOLORS] = {
  OrigamiCubeColor(19, 0, 255,960),
  OrigamiCubeColor(255, 0, 0,1040),
  OrigamiCubeColor(205, 255, 0,0)
};
//TO HERE
unsigned long lastTime = 0;//Store the last time for non-blocking timed events
int current = 0;//track the current index of the array

OrigamiCubeColor nextC = cubeColors[0];//init variable
OrigamiCubeColor currentC = cubeColors[0];//init variable

void setup() {
  pixels.begin();//init the pixel strip
  pixels.setPixelColor(0, pixels.Color(0,0,0));//Set the initial color to black
  pixels.show();//Send the colors to the strip
  updateColorState();
}

void loop() {
    checkTimeout();
}

void updateColorState(){
  nextC = cubeColors[((current +1) % NUMCOLORS)];//variable to store the next color.
  currentC = cubeColors[current];//what is the current color
}

void checkTimeout(){
  //Where we're at
  unsigned long currentTime = millis();
  unsigned long transitionTime = (current>0)?cubeColors[(current-1)].transitionTime:cubeColors[NUMCOLORS-1].transitionTime;//Take the transition time from the previous color - when should this transition
  unsigned long timeElapsed = currentTime - lastTime;//How much time has elapsed since the last update
  float percentage = (float)timeElapsed/(float)transitionTime;//What percentage of time has elapsed - use this to update the transition amount between colors. e.g. 50% between red and blue

  //Where we're going
  //What is the actual color that should be displayed right now. What is 50% between r1 and r2, g1 and g2 and b1 and b2. etc.
  uint32_t diffColor = ((uint32_t)(currentC.r + (int)((float)(nextC.r-currentC.r)*percentage)) << 16) | ((uint32_t)(currentC.g + (int)((float)(nextC.g-currentC.g)*percentage)) <<  8) | (currentC.b + (int)((float)(nextC.b-currentC.b)*percentage));
  pixels.setPixelColor(0, diffColor);//Update the color
  pixels.show();
  
  if (timeElapsed > transitionTime){//If we've hit, or gone passed the target transition time, reset stuff.
    lastTime = currentTime;
    current ++;
    current = current % NUMCOLORS;
    updateColorState();
  }
}

