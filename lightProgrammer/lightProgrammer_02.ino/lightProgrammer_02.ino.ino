#include <Adafruit_NeoPixel.h>
#define PIN            7
#define NUMPIXELS      4
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//Rotary Encoder Variables
int pinA = 3;  // Connected to CLK on KY-040
int pinB = 4;  // Connected to DT on KY-040
int encoderPosCount = 0; 
int pinALast;  
int aVal;
boolean bCW;


int flashMaxSpeed = 1000;
int currentSpeed = flashMaxSpeed/2;
int ledFrames[4] = {8,9,11,10};
int pixelOrder[4] = {0,1,3,2};

String MODE_ROTARYENCODER = "MODE_ROTARYENCODER";
String MODE_POT = "MODE_POT";

String mode = MODE_POT;

void setup() { 
  if (mode == MODE_ROTARYENCODER){
    setupRotary();
  }else{
    setupPot();
  }
  pixels.begin();
} 
void setupRotary(){
  
  pinMode (pinA,INPUT);
  pinMode (pinB,INPUT);
  
  pinALast = digitalRead(pinA);
}
void setupPot(){

}
void loop() {
  for(int i=0;i<NUMPIXELS;i++){
    if (mode == MODE_ROTARYENCODER){
      readRotaryDelay();
    }else{
      readPotDelay();
    }
    pixels.setPixelColor(pixelOrder[i], pixels.Color(255,255,255));
    pixels.show();
    delay(currentSpeed);
    if (currentSpeed > 0){
      pixels.setPixelColor(pixelOrder[i], pixels.Color(0,0,0));
    }
  }
}
void readPotDelay(){
  float analogPotVal = analogRead(2);
  currentSpeed =float(analogPotVal/1024)*flashMaxSpeed;
}
void readRotaryDelay(){
  aVal = digitalRead(pinA);
  if (aVal != pinALast){ // Means the knob is rotating
    // if the knob is rotating, we need to determine direction
    // We do that by reading pin B.
    if (digitalRead(pinB) != aVal) {  // Means pin A Changed first - We're Rotating Clockwise
      currentSpeed+=50;
    } else {// Otherwise B changed first and we're moving CCW
      currentSpeed-=50;
    }
    
    if (currentSpeed > flashMaxSpeed){
      currentSpeed = flashMaxSpeed;
    }else if (currentSpeed < 0){
      currentSpeed = 0;
    }
  }
  pinALast = aVal;
}

