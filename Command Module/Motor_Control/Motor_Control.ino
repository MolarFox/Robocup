/*  Motor Control Sketch
 *  C: 13/08/2017 | LM: 13/08/2017
 *  ----------------------------------
 *  NB: This sketch needs to be converted to library
 *  
 *  Combines magnetometer info with local / global angle commands to steer + drive bot
 *  Global rotation and global drive can be combined with local rotation input
 *  Local drive is manual override, and can be combined with trailing local rotation input (will affect trajectory)
 *  
 *  Note: All angles are written as milliradians
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

// Sets local drive values based on local angle
void localDrive(float driveAngle){
  // Begin by determining which sector of bot angle resides in
  if (driveAngle > 1066.66){        // Angle is past M2
    if (driveAngle < 3600){         // Angle between M2 and M1
      ratM1 = 3200 - driveAngle;
      ratM2 = driveAngle - 1066.66;
      ratM3 = 0;
    }else if (driveAngle < 5333.33){// Angle between M3 and M1
      ratM1 = driveAngle - 3200;
      ratM2 = 0;
      ratM3 = 5333.33 - driveAngle;
    }else if (driveAngle > 5333){   // Angle between M3 and 0 mils
      ratM1 = 0;
      ratM2 = (6400 - driveAngle) + 1066.66;
      ratM3 = driveAngle - 5333.33;
    }
  }else if (driveAngle > -1){       // Angle between 0 mils and M2 (-1 used for continuity)
    ratM1 = 0;
    ratM2 = 1066.66 - driveAngle;
    ratM3 = 1066.66 + driveAngle;
  }

  // Find highest motor value and reduce all to motor drive values based on ratio
  
}


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

