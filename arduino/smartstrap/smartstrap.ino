#include <Adafruit_NeoPixel.h>

#include <Arduino.h>
#include <ArduinoPebbleSerial.h>
#include <stdio.h>
#include <string.h>

#define DATA_PIN 3
#define PIN_FORWARD 5
#define PIN_REVERSE 6
#define PIN_LEFT 11
#define PIN_RIGHT 10
#define PIXEL_PIN 4

#define PIXEL_COUNT 12

static uint8_t s_data_buffer[128];
static long s_last_time = 0;

static const uint16_t s_service_ids[] = {0x0};
static const uint16_t s_attr_ids[] = {0x0};

static int color = 108;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

void goUp(int time){
  // Make sure all other pins are off so we don't short
  digitalWrite(PIN_REVERSE, LOW);
  digitalWrite(PIN_LEFT, LOW);
  digitalWrite(PIN_RIGHT, LOW);
  
  // Turn on forward pin
  digitalWrite(PIN_FORWARD, HIGH);
  if(time < 0){
    delay(100);
  }else{
    delay(time); 
  }
  digitalWrite(PIN_FORWARD, LOW); 
}

void goDown(int time){
  // Make sure all other pins are off so we don't short
  digitalWrite(PIN_FORWARD, LOW);
  digitalWrite(PIN_LEFT, LOW);
  digitalWrite(PIN_RIGHT, LOW);
  
  // Turn on forward pin
  digitalWrite(PIN_REVERSE, HIGH);
  if(time < 0){
    delay(100);
  }else{
    delay(time); 
  }
  digitalWrite(PIN_REVERSE, LOW);
}

void goUpLeft(int time){
  // Make sure all other pins are off so we don't short
  digitalWrite(PIN_REVERSE, LOW);
  digitalWrite(PIN_RIGHT, LOW);
  
  // Turn on forward pin
  digitalWrite(PIN_FORWARD, HIGH);
  digitalWrite(PIN_LEFT, HIGH);
  if(time < 0){
    delay(100);
  }else{
    delay(time); 
  }
  digitalWrite(PIN_FORWARD, LOW);
  digitalWrite(PIN_LEFT, LOW);
}

void goUpRight(int time){
  // Make sure all other pins are off so we don't short
  digitalWrite(PIN_REVERSE, LOW);
  digitalWrite(PIN_LEFT, LOW);
  
  // Turn on forward pin
  digitalWrite(PIN_FORWARD, HIGH);
  digitalWrite(PIN_RIGHT, HIGH);
  if(time < 0){
    delay(100);
  }else{
    delay(time); 
  }
  digitalWrite(PIN_FORWARD, LOW);
  digitalWrite(PIN_RIGHT, LOW);  
}

void goDownRight(int time){
  // Make sure all other pins are off so we don't short
  digitalWrite(PIN_FORWARD, LOW);
  digitalWrite(PIN_LEFT, LOW);
  
  // Turn on forward pin
  digitalWrite(PIN_REVERSE, HIGH);
  digitalWrite(PIN_RIGHT, HIGH);
  if(time < 0){
    delay(100);
  }else{
    delay(time); 
  }
  digitalWrite(PIN_REVERSE, LOW);
  digitalWrite(PIN_RIGHT, LOW);  
}

void goDownLeft(int time){
  // Make sure all other pins are off so we don't short
  digitalWrite(PIN_FORWARD, LOW);
  digitalWrite(PIN_RIGHT, LOW);
  
  // Turn on forward pin
  digitalWrite(PIN_REVERSE, HIGH);
  digitalWrite(PIN_LEFT, HIGH);
  if(time < 0){
    delay(100);
  }else{
    delay(time); 
  }
  digitalWrite(PIN_REVERSE, LOW);
  digitalWrite(PIN_LEFT, LOW);  
}

void showOff(){
  int offset = 0;
  for(int i=0; i<20;i++){
    goUpRight(300);
    doColor(offset);
    offset++;
    delay(100);
    doColor(offset);
    offset++;
    goDownLeft(300);
    doColor(offset);
    offset++;
    delay(100);
    doColor(offset);
    offset++;
    
  }
  for (int i = 0; i < PIXEL_COUNT; i++) {
    strip.setPixelColor((i)%PIXEL_COUNT, Wheel(color));

  }
  strip.show();
  
}

void doColor(int offset){
  for (int i = 0; i < PIXEL_COUNT; i++) {
    strip.setPixelColor((i+offset)%PIXEL_COUNT, Wheel(i*18));
  }
  strip.show();
}

void setup() {
  Serial.begin(9600); 
  pinMode(13, OUTPUT);
  pinMode(PIN_FORWARD, OUTPUT);
  pinMode(PIN_REVERSE, OUTPUT);
  pinMode(PIN_LEFT, OUTPUT);
  pinMode(PIN_RIGHT, OUTPUT);
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  for (int i = 0; i < PIXEL_COUNT; i++) {
    strip.setPixelColor((i)%PIXEL_COUNT, Wheel(color));

  }
  strip.show();
  
  // Setup the Pebble smartstrap connection using one wire software serial
  ArduinoPebbleSerial::begin_software(DATA_PIN, s_data_buffer, sizeof(s_data_buffer),
                                      Baud57600, s_service_ids, 1);
}

void loop() {
  size_t length;
  RequestType type;
  uint16_t service_id;
  uint16_t attribute_id;
  
  // Check to see if a frame was received, and for which service and attribute
  if(ArduinoPebbleSerial::feed(&service_id, &attribute_id, &length, &type)) {
    // We got a frame!
    digitalWrite(13,HIGH);
    if((service_id == 0) && (attribute_id == 0)) {
      // This is a raw data service frame
      s_data_buffer[min(length, sizeof(s_data_buffer))] = '\0';
      Serial.println((char*)s_data_buffer);
      
      if(strcmp((char*)s_data_buffer, "KEY_UP") == 0) {
        // Make sure all other pins are off so we don't short
        digitalWrite(PIN_REVERSE, LOW);
        digitalWrite(PIN_LEFT, LOW);
        digitalWrite(PIN_RIGHT, LOW);
        Serial.println("help");
        // Turn on forward pin
        digitalWrite(PIN_FORWARD, HIGH);
        delay(100);
        digitalWrite(PIN_FORWARD, LOW);
      } else if(strcmp((char*)s_data_buffer, "KEY_DOWN") == 0) {
        // Make sure all other pins are off so we don't short
        digitalWrite(PIN_FORWARD, LOW);
        digitalWrite(PIN_LEFT, LOW);
        digitalWrite(PIN_RIGHT, LOW);
        
        // Turn on forward pin
        digitalWrite(PIN_REVERSE, HIGH);
        delay(100);
        digitalWrite(PIN_REVERSE, LOW);
      } else if(strcmp((char*)s_data_buffer, "KEY_UP_LEFT") == 0) {
        // Make sure all other pins are off so we don't short
        digitalWrite(PIN_REVERSE, LOW);
        digitalWrite(PIN_RIGHT, LOW);
        
        // Turn on forward pin
        digitalWrite(PIN_FORWARD, HIGH);
        digitalWrite(PIN_LEFT, HIGH);
        delay(100);
        digitalWrite(PIN_FORWARD, LOW);
        digitalWrite(PIN_LEFT, LOW);
      } else if(strcmp((char*)s_data_buffer, "KEY_DOWN_LEFT") == 0) {
        // Make sure all other pins are off so we don't short
        digitalWrite(PIN_FORWARD, LOW);
        digitalWrite(PIN_RIGHT, LOW);
        
        // Turn on forward pin
        digitalWrite(PIN_REVERSE, HIGH);
        digitalWrite(PIN_LEFT, HIGH);
        delay(100);
        digitalWrite(PIN_REVERSE, LOW);
        digitalWrite(PIN_LEFT, LOW);
      } else if(strcmp((char*)s_data_buffer, "KEY_UP_RIGHT") == 0) {
        // Make sure all other pins are off so we don't short
        digitalWrite(PIN_REVERSE, LOW);
        digitalWrite(PIN_LEFT, LOW);
        
        // Turn on forward pin
        digitalWrite(PIN_FORWARD, HIGH);
        digitalWrite(PIN_RIGHT, HIGH);
        delay(100);
        digitalWrite(PIN_FORWARD, LOW);
        digitalWrite(PIN_RIGHT, LOW);
      } else if(strcmp((char*)s_data_buffer, "KEY_DOWN_RIGHT") == 0) {
        // Make sure all other pins are off so we don't short
        digitalWrite(PIN_FORWARD, LOW);
        digitalWrite(PIN_LEFT, LOW);
        
        // Turn on forward pin
        digitalWrite(PIN_REVERSE, HIGH);
        digitalWrite(PIN_RIGHT, HIGH);
        delay(100);
        digitalWrite(PIN_REVERSE, LOW);
        digitalWrite(PIN_RIGHT, LOW);
      } else if(strcmp((char*)s_data_buffer, "KEY_LEFT") == 0) {
        // Make sure all other pins are off so we don't short
        digitalWrite(PIN_FORWARD, LOW);
        digitalWrite(PIN_REVERSE, LOW);
        digitalWrite(PIN_RIGHT, LOW);
        
        // Turn on left pin
        digitalWrite(PIN_LEFT, HIGH);
        delay(100);
        digitalWrite(PIN_LEFT, LOW);
      } else if(strcmp((char*)s_data_buffer, "KEY_RIGHT") == 0) {
        // Make sure all other pins are off so we don't short
        digitalWrite(PIN_FORWARD, LOW);
        digitalWrite(PIN_REVERSE, LOW);
        digitalWrite(PIN_LEFT, LOW);
        
        // Turn on right pin
        digitalWrite(PIN_RIGHT, HIGH);
        delay(100);
        digitalWrite(PIN_RIGHT, LOW);
      } else if(strcmp((char*)s_data_buffer, "KEY_SHOW_OFF") == 0) {
        showOff();  
      } else if(strcmp((char*)s_data_buffer, "KEY_GIRL") == 0) {
        for (int i = 0; i < PIXEL_COUNT; i++) {
          color = 250;
          strip.setPixelColor((i)%PIXEL_COUNT, Wheel(color));
    
        }
        strip.show();
      } else if(strcmp((char*)s_data_buffer, "KEY_BOY") == 0) {
        for (int i = 0; i < PIXEL_COUNT; i++) {
          color = 108;
          strip.setPixelColor((i)%PIXEL_COUNT, Wheel(color));
    
        }
        strip.show();
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
