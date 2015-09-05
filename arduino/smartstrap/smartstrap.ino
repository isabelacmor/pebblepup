#include <Arduino.h>
#include <ArduinoPebbleSerial.h>
#include <stdio.h>
#include <string.h>

#define DATA_PIN 3
#define PIN_FORWARD 5
#define PIN_REVERSE 6
#define PIN_LEFT 10
#define PIN_RIGHT 11

static uint8_t s_data_buffer[128];
static long s_last_time = 0;

static const uint16_t s_service_ids[] = {0x0};
static const uint16_t s_attr_ids[] = {0x0};

void setup() {
  Serial.begin(9600); 
  pinMode(13, OUTPUT);
  pinMode(PIN_FORWARD, OUTPUT);
  pinMode(PIN_REVERSE, OUTPUT);
  pinMode(PIN_LEFT, OUTPUT);
  pinMode(PIN_RIGHT, OUTPUT);
  
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
    if((service_id == 0) && (attribute_id == 0)) {
      // This is a raw data service frame
      s_data_buffer[min(length, sizeof(s_data_buffer))] = '\0';
      Serial.println((char*)s_data_buffer);
      
      if(strcmp((char*)s_data_buffer, "KEY_UP") == 0) {
        // Make sure all other pins are off so we don't short
        digitalWrite(PIN_REVERSE, LOW);
        digitalWrite(PIN_LEFT, LOW);
        digitalWrite(PIN_RIGHT, LOW);
        
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
      }
      
//      if(type == RequestTypeRead || type == RequestTypeWriteRead) {
//        // Write the data to send
//        snprintf((char*)s_data_buffer, sizeof(s_data_buffer), "Hello, Pebble!");
//        Serial.print("DRIVE");
//        // Send the data
//        //ArduinoPebbleSerial::write(true, s_data_buffer, strlen((char*)s_data_buffer)+1);
//      }
    }
  }

//  if (ArduinoPebbleSerial::is_connected()) {
//    digitalWrite(13, HIGH);
//    
//    // Send the data to the watch every second
//    long now = millis();
//    if(now - s_last_time > 5000) {
//      s_last_time = now;
//      // Notify the watch
//      ArduinoPebbleSerial::notify(s_service_ids[0], s_attr_ids[0]);
//    }
//  } else {
//    digitalWrite(13, LOW);
//  }
}
