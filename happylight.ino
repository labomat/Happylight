#include "FastLED.h"
#include <SharpIR.h>

FASTLED_USING_NAMESPACE

#define DATA_PIN    7
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    22
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          48
#define FRAMES_PER_SECOND  60

#define FAR 3
#define MEDIUM 2
#define NEAR 1

int state = 3;

int ledState = LOW;         // ledState used to set the LED
int distance = 0; 
int interval = 10;          // interval at which to blink (milliseconds)

unsigned long previousMillis = 0;        // will store last time LED was updated

int fadeAmount = 5;  // Set the amount to fade I usually do 5, 10, 15, 20, 25 etc even up to 255.
int brightness = 0; 

SharpIR sensor( SharpIR::GP2Y0A21YK0F, A0 );

void setup() {
  //delay(15000); // 3 second delay for recovery
  
  // FastLED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  leds[0] = CRGB::Red;

  for( int i = 1; i < 23; i++) {
    leds[i] = CRGB(255,245,200);
  }
    
  FastLED.show();  

  Serial.begin(9600); 
}

  
void loop()
{

  int value = 0;
  
  for (int i = 0; i < 20; i++) {
    value = value + sensor.getDistance();
    //Serial.println(i);  
    //Serial.println(sensor.getDistance());   
    //delay(10);
  }
  distance = value/20;
  
  //Serial.print("Distance: ");
  //Serial.println(distance);     
//  Serial.print("cm | State: ");
//  Serial.println(state);

  if (distance > 65) state = FAR;
  if ((distance < 65)&&(distance > 40)) state = MEDIUM;
  if (distance < 40) state = NEAR;

  switch(state)
  {
    case FAR:
      for( int i = 1; i < 23; i++) {
        leds[i] = CRGB(255,245,170);
      }
      FastLED.show();  
      delay(interval*50); 
      
    break;
  
    case MEDIUM:

    leds[0] = CRGB::Red;
    
    for( int i = 15; i < 23; i++) {
      leds[i] = CRGB(64,160,32);
    }
    
    FastLED.show();  
    delay(interval*50); 
    break;
  
    case NEAR:
    
    leds[0] = CRGB::Red;
    leds[0].fadeLightBy(brightness);
  
    // send the 'leds' array out to the actual LED strip
    FastLED.show();  

    interval = distance*0.7;

    //Serial.print(brightness);
    //Serial.print(" | ");
    //Serial.println(interval);

    
    brightness = brightness + fadeAmount;
    if(brightness == 0 || brightness > 254)
    {
      delay(interval); 
      fadeAmount = -fadeAmount ; 
    }    
    // insert a delay to keep the framerate modest
    delay(interval); 
  
    break;
  
    }
}


