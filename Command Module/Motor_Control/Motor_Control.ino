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
float offM1, offM2, offM3;  // Offset of each motor when driven (for rotation) (%)
int maxDrive = 75;          // Value of most activated motor (%)


void setup() {
  mag.begin(); // Initialise magnetometer
  Serial.begin(9600); // TEMP
  localDrive(6400);
  Serial.print("M1 - "); Serial.println(M1);
  Serial.print("M2 - "); Serial.println(M2);
  Serial.print("M3 - "); Serial.println(M3);
}

void loop() {
  relaxMag(); // Refreshes current mag readings (uses internally scheduled delay)
  
}

// Sets local drive values based on local angle (mils)
void localDrive(float driveAngle){
  float scaledownFactor; // Holds value to divide ratios into maxDrive percentage
  bool runReversed[3];  // Holds values for motor direction
  
  // Begin by determining which sector of bot angle resides in
  if (driveAngle > 1066.66){        // Angle is past M2
    if (driveAngle < 3600){         // Angle between M2 and M1
      runReversed[0] = false;
      runReversed[1] = true;
      runReversed[2] = false;
      ratM1 = 3200 - driveAngle;
      ratM2 = driveAngle - 1066.66;
      ratM3 = 0;
    }else if (driveAngle < 5333.33){// Angle between M3 and M1
      runReversed[0] = true;
      runReversed[1] = false;
      runReversed[2] = false;
      ratM1 = driveAngle - 3200;
      ratM2 = 0;
      ratM3 = 5333.33 - driveAngle;
    }else if (driveAngle > 5333){   // Angle between M3 and 0 mils
      runReversed[0] = false;
      runReversed[1] = false;
      runReversed[2] = true;
      ratM1 = 0;
      ratM2 = (6400 - driveAngle) + 1066.66;
      ratM3 = driveAngle - 5333.33;
    }
  }else if (driveAngle > -1){       // Angle between 0 mils and M2 (-1 used for continuity)
    runReversed[0] = false;
    runReversed[1] = false;
    runReversed[2] = true;
    ratM1 = 0;
    ratM2 = 1066.66 - driveAngle;
    ratM3 = 1066.66 + driveAngle;
  }

  // Find highest motor value and reduce all to motor drive values based on ratio
  if (ratM1 > ratM2){
    if (ratM1 > ratM3){ // M1 greatest
      scaledownFactor = ratM1 / maxDrive;
    }else{              // M3 greatest
      scaledownFactor = ratM3 / maxDrive;
    }
  }else{
    if (ratM2 > ratM3){ // M2 greatest
      scaledownFactor = ratM2 / maxDrive;
    }else{              // M3 greatest
      scaledownFactor = ratM3 / maxDrive;
    }
  }

  // Convert to motor drive percentages and compensate for direction
  M1 = ratM1 / scaledownFactor;
  M2 = ratM2 / scaledownFactor;
  M3 = ratM3 / scaledownFactor;

  if (runReversed[0]) M1 = -1 * M1;
  if (runReversed[1]) M2 = -1 * M2;
  if (runReversed[2]) M3 = -1 * M3;
}


// Sets local rotation offset (-100 <-> 100) (about bot central axis)
void localRot(float rotMagnitude){
  offM1 = rotMagnitude;
  offM2 = rotMagnitude;
  offM3 = rotMagnitude;
}


// Drives motors
void driveAll(void){
  // Add rotation offsets
  M1 = M1 + offM1;
  M2 = M2 + offM2;
  M3 = M3 + offM3;

  // Drive motors
  if (M1 > 0) analogWrite(drv[1], M1);
  else analogWrite(drv[0], M1);
  
  if (M2 > 0) analogWrite(drv[3], M2);
  else analogWrite(drv[2], M2);
  
  if (M3 > 0) analogWrite(drv[5], M3);
  else analogWrite(drv[4], M3);
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

