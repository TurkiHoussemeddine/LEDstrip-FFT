/*
  Copyright (c) 2014-2015 NicoHood
  See the readme for credit to other people.

  MSGEQ7 Bass example

  Output via Bass (choose a PWM pin!)
  Fast setup to demonstrate functionality

  Reads MSGEQ7 IC with 7 different frequencies from range 0-255
  53Hz, 150Hz, 400Hz, 1kHz, 2.5kHz, 5.25KHz, 15kHz
*/
// WS218B
#include <FastLED.h>
#define LED_PIN     7
#define NUM_LEDS    240
CRGBArray<NUM_LEDS> leds; //init LED array
// MSGEQ7
#include "MSGEQ7.h" 
#define pinAnalog0 A0
#define pinAnalog1 A1
#define pinReset 5
#define pinStrobe 4
#define MSGEQ7_INTERVAL ReadsPerSecond(50)
#define MSGEQ7_SMOOTH 50 // Range: 0-255

CMSGEQ7<MSGEQ7_SMOOTH, pinReset, pinStrobe, pinAnalog0 , pinAnalog1> MSGEQ7;

// Choose a PWM pin!
#define pinBass 11
#define pinTreb 10
#define pinMid 9

void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  // This will set the IC ready for reading
  MSGEQ7.begin();
  // led setup
  pinMode(pinBass, OUTPUT);
  pinMode(pinTreb, OUTPUT);
  pinMode(pinMid, OUTPUT);
  Serial.begin(9600);
}

void loop() { 

  // Analyze without delay every interval
  bool newReading = MSGEQ7.read(MSGEQ7_INTERVAL);

  // led output
  if (newReading) {
    
    // Read frequency
    uint8_t inputbass = mapNoise(MSGEQ7.get(1,0))+mapNoise(MSGEQ7.get(1,1)) >> 1;
    uint8_t inputtreb = mapNoise(MSGEQ7.get(5,0))+mapNoise(MSGEQ7.get(5,1)) >> 1; 
    uint8_t inputmid = mapNoise(MSGEQ7.get(4,0))+mapNoise(MSGEQ7.get(4,1)) >> 1;
  
    if(inputbass < 10) inputbass = 0;   
    else inputbass = map(inputbass,10,1023,0,255); 
    // Output PWM signal via Bass to the music beat
    // analogWrite(pinBass, inputbass);
    inputbass = map(inputbass,0,30,0,255); 
    if(inputtreb < 50) inputtreb = 0;
    else inputtreb = map(inputtreb,50,1023,0,255); 
    //analogWrite(pinTreb, inputtreb);
    inputtreb = map(inputtreb,0,30,0,255);
    ////////////////////////////////////////////////
    // out to different parts of led strip
    for(int i =0 ; i<=52;i++){
    leds[i]= CHSV(0,255,inputbass);
    }
    for(int i = 53 ; i<=94;i++){
    leds[i]= CHSV(45,255,inputtreb);
    }
    for(int i =95 ; i<=163;i++){
    leds[i]= CHSV(0,255,inputbass);
    }
    for(int i = 164 ; i<=208;i++){
    leds[i]= CHSV(45,255,inputtreb);
    }
    for(int i =209; i<=240;i++){
    leds[i]= CHSV(0,255,inputbass);
    }
    /*if(inputmid < 40) inputmid =0; 
    else inputmid = map(inputmid,40,1023,0,255); 
    analogWrite(pinMid, inputmid);  
    */

   //debug
    Serial.print("inputbass :"); 
    Serial.print(inputbass);
    Serial.print("\t");
    Serial.print(" inputtreb :");
    Serial.print(inputtreb);
    Serial.print("\n");
    FastLED.show(); //printing
    }
 
   
  
  }
