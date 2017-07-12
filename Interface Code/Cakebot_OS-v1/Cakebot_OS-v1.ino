/*  Cakebot OS screen manager
 *  C: 12/07/2017 | LM: 12/07/2017
 *  ----------------------------------
 *  Reads in sensor info via serial from command unit and outputs to screen
 *  Touchscreen hardware unreliable - not using touch
 *  ----------------------------------
 *  By Rithesh R Jayaram
 *  ----------------------------------
 *  Based on modified Adafruit tftPaint sketch
 *  Using modified Adafruit touch and tft display libraries
*/

// LIBRARIES //
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
//#include <TouchScreen.h>    // Touchscreen library

// Pin Definitions //
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET); // Initialise TFT LCD instance

// Common Colour Codes //
#define THEMECOLOUR 0x07E0
#define RED         0xF800
#define WHITE       0xFFFF
#define BLACK       0x0000



void setup() {
  tft.reset();
  uint16_t identifier = tft.readID(); // Gets driver number from device
  tft.begin(identifier);  // Inits for detected device
  tft.fillScreen(BLACK);
  tft.setRotation(3);

  // Screen (window) borders
  tft.drawRect(0, 0,  320, 16,  THEMECOLOUR);
  tft.drawRect(0, 15, 320, 225, THEMECOLOUR);

  // Titlebar dividers
  tft.drawLine(19, 0, 19, 15, THEMECOLOUR);

  // Team CI Icon
  tft.setCursor(5, 4);
  tft.setTextColor(0x07FF);  tft.setTextSize(1);
  tft.print("CI");
  tft.drawRect(3, 3, 14, 10, 0xF81F);

  // Title Bar text
  tft.setCursor(23, 4);
  tft.setTextColor(WHITE);  tft.setTextSize(1);
  tft.print("CAKEBOT OS v1.0");
  tft.setTextColor(0xFFE0);  tft.setTextSize(1);
  tft.println("    - COMD MODULE VALS -");

  // Battery icon (TODO: Remove)
  tft.drawRect(292, 2, 25, 11, RED);
}

void loop() {
  
}
