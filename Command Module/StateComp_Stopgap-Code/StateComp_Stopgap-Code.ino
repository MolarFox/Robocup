/*  Robocup State Competition code
 *  C: 14/08/2017| LM: 14/08/2017
 *  ----------------------------------
 *  <Description>
 *  ----------------------------------
 *  By Rithesh R Jayaram
 *  ----------------------------------
 *  Based on Adafuit HMC885l library and TRIS10 TSOP31140 code
*/

// LIBRARIES //
#include <Wire.h> // I2C used for magnetometer and TSOPs
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345); // Instantiate magnetometer


// STATIC VARIABLES //
#define magReadDelay 2  // Minimum delay between magnetometer readings (milliseconds)
// Pinouts of motors:    1CCW 1CW | 2CCW 2CW | 3CCW 3CW
const int drv[6] =       {3,   2,    5,   4,    7,   6};


// SENSOR DATA CACHE //
float milHeading;       // Raw value (in millirads) from magnetometer
int colourVal[3];       // Raw colour values from colour sensors

int colourWhite[3];     // Recorded value for field green for each sensor
int colourGreen[3];     // Recorded value for field line for each sensor
float enemyGoal;        // Direction of enemy goal at calibration

float M1, M2, M3;           // Raw motor drive values (%)
int maxDrive = 30;      // Value of most activated motor (%)

void setup() {
  mag.begin(); // Initialise magnetometer
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH); // Activate colour sensor lights
  delay(5);
  relaxColour();  // Refresh colour sensor readings
  
  //TEMP
  Serial.begin(9600);
  
}

void loop() {
  relaxMag();     // Refreshes current mag readings (uses internally scheduled delay)
  relaxColour();  // Refreshes current colour sensor readings (~300ns per cycle)


  
  //TEMP
  Serial.println(colourVal[0]);
  Serial.println(colourVal[1]);
  Serial.println(colourVal[2]);
  Serial.println();
  delay(500);
}



/* --- ROUTINES --------------------------------------------------------------------------*/



/* --- MOTORS ----------------------------------------------------------------------------*/



// Drives motors
void driveAll(void){
  // Drive motors
  if (M1 >= 0) analogWrite(drv[1], M1);
  else if (M1 < 0) analogWrite(drv[0], M1);
  
  if (M2 >= 0) analogWrite(drv[3], M2);
  else if (M2 < 0) analogWrite(drv[2], M2);
  
  if (M3 >= 0) analogWrite(drv[5], M3);
  else if (M3 < 0) analogWrite(drv[4], M3);
}


// Drives bot forwards/backwards
void driveForward(int magnitude){
  M1 = 0;
  M2 = magnitude;
  M3 = -1 * magnitude;
}

// Rotates bot in place
void rotateStatic(int magnitude){
  M1 = magnitude;
  M2 = magnitude;
  M3 = magnitude;
}

/* --- SENSORS ---------------------------------------------------------------------------*/
// Relaxes information from colour sensors
void relaxColour(void){
  colourVal[0] = analogRead(A0);
  colourVal[1] = analogRead(A7);
  colourVal[2] = analogRead(A13);
}

// Relaxes information from magnetometer (internal delay)
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
