/**
  TRIS10 ROBOTICS (https://tris10.com)
  AUTHOR:   Tristan Roberts
  DATE:     26-09-2014
  
  A simple program for an Arduino to act as an I2C-enabled pulsed IR sensor.
  Designed to use the TSOP(3)1140 from Vishay Technologies.

  This implements a very simple algorithm that is by no means perfect.
  As such it should be used as a base to build upon and to test with.
  Possiblee algorithms include doing curve normalisation, or doing a repeating
  search for the best group of senosrs. Remember to cover the TSOPs well though!

  I2C DOCUMENTATION:
  Address: 0xA0 (8-bit), 0x50 (7-bit)

  To call, simply use:
  Wire.requestFrom(0x50, 1);
  if (Wire.available()) {
    Serial.println(Wire.read());
  }
  
  (No register setting required, or possible)
*/
#include <Wire.h>

// * The maximum value (used to average readings)
// * Note: the greater, the more accurate but slower (max = 255)
#define MAX_COUNTER 30

// * The number of sensors on the robot.
// * Tip: make this a factor of 360
#define SENSORS 7

// * The pin used for powering all of the sensors (in parallel)
#define UNLOCK_PIN 52

// * The 8-bit I2C address to use.  See header doc.
#define ADDRESS_8 0xA0
#define ADDRESS_7 (ADDRESS_8 >> 1)

// * Enable or disable debug info
// * No noticable performance impact normally.
// * To enable, set to 1.  To disable, set to 0
#define DEBUG 1

// * The baud rate to use for debugging output
// * Keep this low to avoid communication errors
// * Only relevant if DEBUG == 1
#define DEBUG_BAUD_RATE 9600

// * Sensor to Arduino pin mappings 
// * Note: Analog sensors follow on from digital pins (or use A0 notation)
// * If a sensor is broken set it to 255 to disable it
                //SENSOR: 1   2   3   4   5   6   7
byte sensors[SENSORS] = { 255, 255, 24, 28, 25, 26, 27};

// ** END CONFIG **

// * The best IR sensor currently (0 means no sensor)
// * Invariant: 0 <= IR_SENSOR <= SENSORS <= 255
byte IR_SENSOR = 0;

// * The number of loops so far
// * Invariant: 0 <= counter <= MAX_COUNTER <= 255
byte counter = 0;

// * The current values for each sensor (0 to MAX_COUNTER)
byte values[SENSORS];  

/**
  Receives an I2C read request and sends back the best sensor.
  Note if you want to add more read capabilities, onReceive() interrupts must
  be configureed as well and register maps.

  See 
*/
void i2c_request() {
  Wire.write(IR_SENSOR);
}

void setup() {
  // Initialize I2C slave
  Wire.begin(ADDRESS_7);
  Wire.onRequest(i2c_request);
  
  // Enable Debug
  if (DEBUG) {
    Serial.begin(DEBUG_BAUD_RATE);
  }
  
  // Initialize the diodes and enable power
  pinMode(UNLOCK_PIN, OUTPUT);
  digitalWrite(UNLOCK_PIN, HIGH);
  for (byte i = 0;i < SENSORS;i++) {
    values[i] = 0;
    pinMode(sensors[i], INPUT);
  } 
}

void loop() {
  delay(5);
  // Read the sensors and get the current reading
  for (byte i = 0;i < SENSORS;i++) {
    if (sensors[i] < 255) {
      values[i] += (digitalRead(sensors[i]) == HIGH ? 0 : 1); 
    }
  }
  
  counter++;
  
  // If finished a block, calculate the best.
  if (counter >= MAX_COUNTER) {
    counter = 0;
    
    // * the index of the best sensor
    byte b_index = 0;
    
    // * the reading of the best sensor
    byte b_value = 0;
    
    for (byte i = 0;i < SENSORS;i++) {
      if (values[i] > b_value) {
        b_index = i + 1; // human readable: diode 1, 2, etc 
        b_value = values[i];
      }

      // debug stuff - disable in competition as it slows it down
      if (DEBUG) {
        Serial.print(i + 1, DEC);
        Serial.print("=");
        Serial.print(values[i], DEC);
        Serial.print(" ");
      }

      values[i] = 0;
    }
    
    // set the new best sensor
    IR_SENSOR   = b_index;

    // In old version of Arduino, counter may not be reset - not sure why?
    counter = 0;
   
     // reset all sensors by dropping power for 2ms
    digitalWrite(UNLOCK_PIN, LOW);
    delay(2);
    digitalWrite(UNLOCK_PIN, HIGH);
    
    
    // more debug
    if (DEBUG) {
      Serial.print("BEST = ");
      Serial.print(b_index, DEC);
      Serial.print(" (");
      Serial.print(sensors[b_index-1], DEC);
      Serial.print("): ");
      Serial.println(b_value, DEC);
    }
  }
}

