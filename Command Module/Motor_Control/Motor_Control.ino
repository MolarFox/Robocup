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

// STATIC VARIABLES //
#define magReadDelay 2  // Minimum delay between magnetometer readings (milliseconds)
// Pinouts of motors:    1CCW 1CW | 2CCW 2CW | 3CCW 3CW
const int drv[6] =       {2,   3,    4,   5,    6,   7};

// VARIABLES //
byte rotConType= 0;     // Current rotation mode (0 = auto, 1 = local, 2 = global)
byte driveConType = 0;  // Current drive    mode (0 = auto, 1 = local, 2 = global)

float milHeading;       // Raw value (in millirads) from magnetometer
int locRotAngle, globRotAngle, locDriveAngle, globDriveAngle; // Values for control vectors

float ratM1, ratM2, ratM3;  // Motor drive ratios
float M1, M2, M3;           // Raw motor drive values (%)
int maxDrive;               // Value of most activated motor (%)


void setup() {
  mag.begin(); // Initialise magnetometer
  Serial.begin(9600); // TEMP
}

void loop() {
  relaxMag(); // Refreshes current mag readings
  Serial.println(milHeading); // TEMP
  
}

// Sets 

// Relaxes information from magnetometer
void relaxMag(void){
  float heading;          // Holds heading value in radians
  static float nextRun;   // Holds next runtime (millis)
  
  if (millis() > nextRun){
    sensors_event_t event; // Gets new magnetometer event
    mag.getEvent(&event);
    heading = atan2(event.magnetic.y, event.magnetic.x);  // Store new vals in variable (does not account for tilt)
    milHeading = heading * 1000;            // Convert to milliradians for global variable
    if (milHeading < 0) milHeading += 6400; // Compensate for negative reading
    
    nextRun = millis() + magReadDelay;  // Schedule next refresh
  }
}

