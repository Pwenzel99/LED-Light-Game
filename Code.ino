#include "LPD8806.h"
#include "SPI.h" // Comment out this line if using Trinket or Gemma
#ifdef __AVR_ATtiny85__
 #include <avr/power.h>
#endif

int nLEDs = 32;

int dataPin  = 9;
int clockPin = 10;

const int butt1Pin = 5;
const int butt2Pin = 6;

int track=1;

boolean endGame = false;
boolean butt1win = false;

int pacer = 250;


LPD8806 strip = LPD8806(nLEDs, dataPin, clockPin);

void setup() {
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif

  // Start up the LED strip
  strip.begin();
  for(int i=0; i<strip.numPixels();i++)
  {
    strip.setPixelColor(i,strip.Color(0,0,20));
  }
  strip.setPixelColor(0, strip.Color(10,10,10));
  strip.setPixelColor(strip.numPixels()-1, strip.Color(10,0,10));

  // Update the strip, to start they are all 'off'
  strip.show();

  pinMode(butt1Pin, INPUT);
  pinMode(butt2Pin, INPUT);
}

void endGameFunction(){
  if(track < 16){
    for(int i=0; i<=strip.numPixels()-1; i+=1){
      strip.setPixelColor(i, 0);
      strip.show();
      delay(100);
    }
  }
  else
  {
      for(int i=strip.numPixels()-1; i>=0; i--){
      strip.setPixelColor(i, 0);
      strip.show();
      delay(100);
    }
  }
}

void runGame(uint8_t wait){
    boolean butt1notpressed = false;
    boolean butt2notpressed = true;
    uint32_t c = strip.Color(20,20,20);
    uint32_t c2 = strip.Color(20,20,20);
    int butt1State = digitalRead(butt1Pin);
    int butt2State = digitalRead(butt2Pin);

    if(!endGame){
    for(int i=track; i<=strip.numPixels()-1; i+=1){
      butt1State = digitalRead(butt1Pin);
      butt2State = digitalRead(butt2Pin);
      if(i==strip.numPixels()-1)
      {
        endGame = true;
        break;
      }
      else if(i<5 || i>26)
    {
      c = strip.Color(255,0,0);
      c2 = strip.Color(20,0,0);
      if(i<strip.numPixels()-1&&i>26){
        if(butt2State==HIGH && butt2notpressed){
          track = i;
          butt2notpressed = false;
          break;
        }
      }
    }
     else if(butt1State==HIGH && butt1notpressed){
      track = 31;
      endGame = true;
      break;
    }
    else if(butt2State==HIGH && butt2notpressed){
      track = 0;
      endGame =true;
      break;
    }
     else if(i<10 || i > 21)
    {
      c = strip.Color(100,100,0);
      c2 = strip.Color(10,10,0);
    }
    else
    {
      c= strip.Color(0,255,0);
      c2 = strip.Color(0,20,0);
    }
      strip.setPixelColor(i, c);
      if(i!=1)
      {
        strip.setPixelColor(i-1, c2);
        if(i!=2)
        {
          strip.setPixelColor(i-2, strip.Color(0,0,40));
        }
      }
      strip.show();
      delay(wait);
    }
    }
    wait = wait/3*2;
    pacer = pacer/3*2;
    if(!endGame){
      butt1notpressed = true;
    for(int i=track; i>=0; i-=1){
      butt1State = digitalRead(butt1Pin);
      butt2State = digitalRead(butt2Pin);
      if(i==0)
      {
        endGame = true;
        break;
      }
    else if(i<5 || i>26)
    {
      c = strip.Color(255,0,0);
      c2 = strip.Color(20,0,0);
      if(i>0&&i<5){
        if(butt1State==HIGH && butt1notpressed){
          track = i;
          butt1notpressed = false;
          break;
        }
      }
    }
     else if(butt1State==HIGH && butt1notpressed){
      track = 31;
      endGame = true;
      break;
    }
    else if(butt2State==HIGH && butt2notpressed){
      track = 0;
      endGame =true;
      break;
    }
    else if(i<10 || i > 21)
    {
      c = strip.Color(100,100,0);
      c2 = strip.Color(10,10,0);
    }
    else
    {
      c= strip.Color(0,255,0);
      c2 = strip.Color(0,20,0);
    }
      strip.setPixelColor(i, c);
      
      if(i!= strip.numPixels()-2)
      {
        strip.setPixelColor(i+1, c2);
        if(i!=strip.numPixels()-3)
        {
          strip.setPixelColor(i+2,strip.Color(0,0,40));
        }
      }
      strip.show();
      delay(wait);
    }
    }
    if(endGame){
      delay(100);
      endGameFunction();
    }
}

void loop() {
        runGame(pacer);
        pacer = pacer/3*2;
  // put your main code here, to run repeatedly:
  
}
