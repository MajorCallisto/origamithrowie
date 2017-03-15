#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN            1
#define NUMPIXELS      1

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

unsigned long lastTime = 0;

float myColors[] = {2, 4, 8, 3, 6};

class OrigamiCubeColor{
  public: int r = 0;
  public: int g = 0;
  public: int b = 0;
  public: unsigned long triggerTime = 0;
  public: uint32_t color = 0;
  public: String colorStatus = "";
  
  public:
  OrigamiCubeColor(int _r, int _g, int _b, unsigned long _triggerTime){
    r =_r;
    g=_g;
    b=_b;
    triggerTime = _triggerTime;

    color = ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
  }
};

#define NUMCOLORS 3
OrigamiCubeColor cubeColors[NUMCOLORS] = {OrigamiCubeColor(255,0,255,100),OrigamiCubeColor(255,255,0,100),OrigamiCubeColor(0,255,255,100)};

int current = 0;

void setup() {
  pixels.begin();
  pixels.setPixelColor(0, pixels.Color(0,0,0));
  pixels.show();
}

void loop() {
    checkTimeout();
}
void checkTimeout(){
  unsigned long currentTime = millis();
  unsigned long triggerTime = (current>0)?cubeColors[(current-1)].triggerTime:cubeColors[NUMCOLORS-1].triggerTime;
  unsigned long timeElapsed = currentTime - lastTime;
  float percentage = (float)timeElapsed/(float)triggerTime;
  OrigamiCubeColor nextC = cubeColors[((current +1) % NUMCOLORS)];
  OrigamiCubeColor currentC = cubeColors[current];
  uint32_t diffColor = ((uint32_t)(currentC.r + (int)((float)(nextC.r-currentC.r)*percentage)) << 16) | ((uint32_t)(currentC.g + (int)((float)(nextC.g-currentC.g)*percentage)) <<  8) | (currentC.b + (int)((float)(nextC.b-currentC.b)*percentage));
  pixels.setPixelColor(0, diffColor);
  pixels.show();
  if (timeElapsed > triggerTime){
    //pixels.setPixelColor(0, cubeColors[current].color);
    //pixels.show();
    lastTime = currentTime;
    current ++;
    current = current % NUMCOLORS;
  }
}

