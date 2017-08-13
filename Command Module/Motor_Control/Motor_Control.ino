/*  Motor Control Sketch
 *  C: 13/08/2017 | LM: 13/08/2017
 *  ----------------------------------
 *  NB: This sketch needs to be converted to library
 *  ----------------------------------
 *  By Rithesh R Jayaram
 *  ----------------------------------
 *  Uses Adafuit HMC588l Magnetometer Library
*/

// LIBRARIES //
#include <Wire.h> // I2C used only for magnetometer
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345); // Instantiate magnetometer

// VARIABLES //
byte rotConType= 0;     // Current rotation mode (0 = auto, 1 = local, 2 = global)
byte driveConType = 0;  // Current drive    mode (0 = auto, 1 = local, 2 = global)

int locRotAngle, globRotAngle, locDriveAngle, globDriveAngle; // Values for control vectors

signed int M1, M2, M3;  // Motor drive values


void setup() {
  mag.begin(); // Initialise magnetometer
}

void loop() {
  relaxMag(); // Refreshes current mag readings
}


// Relaxes information from magnetometer
void relaxMag(void){
  float heading;    // Holds heading value in radians
  float milHeading; // Holds heading value in mils
  
  sensors_event_t event; // Gets new magnetometer event
  mag.getEvent(&event);
  heading = atan2(event.magnetic.y, event.magnetic.x);  // Store new vals in variable (does not account for tilt)
  milHeading = heading * 180/M_PI;
}

