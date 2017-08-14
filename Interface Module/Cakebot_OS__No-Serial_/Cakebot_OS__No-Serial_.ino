/*  Cakebot OS Screen Filler
 *  C: 12/07/2017 | LM: 12/07/2017
 *  ----------------------------------
 *  This code does not interact with serial in any way - it is simply a demo 
 *  showcase of the GUI functions
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
#define ORANGE      0xFF00
#define YELLOW      0xFFE0
#define GREEN       0x07E0
#define RED         0xF826
#define WHITE       0xFFFF
#define BLACK       0x0000
#define PINK        0xF8FA

// Battery voltage levels (millivolts) (adjust as necessary)
#define lowVolt     7000
#define highVolt    8400

void setup() {
  tft.reset();
  uint16_t identifier = tft.readID(); // Gets driver number from device
  tft.begin(identifier);  // Inits for detected device
  tft.fillScreen(BLACK);
  tft.setRotation(3);

  drawMainWindow("    - COMD MODULE MONITOR -",0xFFE0, GREEN);  // Draws menubar and screen borders
  drawBotSchematic();

  //Draw loading screen
  tft.setCursor(51, 85);
  tft.setTextColor(ORANGE);  tft.setTextSize(1);
  tft.print("Loading...");
  
  tft.setCursor(10, 180);
  tft.setTextColor(GREEN);  tft.setTextSize(1);
  tft.print("INTERFACE MODULE STATUS:   OK");

  tft.setCursor(10, 200);
  tft.setTextColor(GREEN);  tft.setTextSize(1);
  tft.print("COMMAND MODULE STATUS:"); tft.setTextColor(ORANGE); tft.print("     INITIALISING...");

  tft.setCursor(10, 220);
  tft.setTextColor(GREEN);  tft.setTextSize(1);
  tft.print("SECONDARY MODULE STATUS:"); tft.setTextColor(PINK); tft.print("   -- DISABLED --");

  tft.drawRect(5, 175,  160, 58,  GREEN);
  tft.drawRect(165, 175,  147, 58,  GREEN);

  tft.drawCircle(180, 50, 20, WHITE);
  tft.drawLine  (180, 50, 180, 31 , RED);
  tft.drawCircle(230, 50, 20, WHITE);
  tft.drawLine  (230, 50, 230, 31 , YELLOW);
  tft.drawCircle(280, 50, 20, WHITE);
  tft.drawLine  (280, 50, 280, 31 , WHITE);

  tft.setCursor(172, 78);
  tft.setTextColor(WHITE);  tft.setTextSize(1);
  tft.print("MAG      IR     MOTOR");

  tft.setCursor(140, 125);
  tft.setTextColor(ORANGE);  tft.setTextSize(2);
  tft.print("LOADING SERIAL");

  tft.setCursor(11, 22);
  tft.setTextColor(ORANGE);  tft.setTextSize(2);
  tft.print("##       ##");
  tft.setCursor(25, 150);
  tft.print("##");

  delay(2000);

  // Enter error mode
  tft.fillRect(166, 190,  110, 30,  BLACK);
  tft.fillRect(140, 100,  180, 50,  BLACK);
  tft.fillRect(51, 70,  68, 30,  BLACK);
}


void loop() {
  // RED LOOP
  tft.setCursor(11, 22);
  tft.setTextColor(RED);  tft.setTextSize(2);
  tft.print("##       ##");
  tft.setCursor(25, 150);
  tft.print("##");

  tft.setCursor(130, 125);
  tft.setTextColor(RED);  tft.setTextSize(2);
  tft.print("ERR: NO SERIAL!");

  tft.setCursor(10, 200);
  tft.setTextColor(GREEN);  tft.setTextSize(1);
  tft.print("COMMAND MODULE STATUS:"); tft.setTextColor(RED); tft.print("     NOT RESPONDING!");

  tft.drawTriangle(
    28.29,  39,
    8.29,   73.64,
    21.29,  81.14,
    YELLOW);
}


// Function to draw outer screen borders and menubar (Window title, title colour, border colour)
void drawMainWindow(String winTitle, int winTitleColour, int BorderColour){
  // Screen (window) borders
  tft.fillRect(0, 0,  320, 16,  BLACK);
  tft.drawRect(0, 0,  320, 16,  BorderColour);
  tft.drawRect(0, 15, 320, 225, BorderColour);

  // Titlebar dividers
  tft.drawLine(19, 0, 19, 15, BorderColour);

  // Team CI Icon
  tft.setCursor(5, 4);
  tft.setTextColor(0x07FF);  tft.setTextSize(1);
  tft.print("CI");
  tft.drawRect(3, 3, 14, 10, 0xF81F);

  // Title Bar text
  tft.setCursor(23, 4);
  tft.setTextColor(WHITE);  tft.setTextSize(1);
  tft.print("CAKEBOT OS v1.0");
  tft.setTextColor(winTitleColour);  tft.setTextSize(1);
  tft.println(winTitle);

  drawBatteryIcon();
}


// Function to draw the bot diagram onto the screen
void drawBotSchematic(){
  tft.drawCircle(75, 90, 60, WHITE);  // Bot base outline
  tft.drawCircle(75, 90, 59, WHITE);
  tft.fillRect(55, 142, 40, 15, BLACK);
  tft.drawRect(55, 142, 40, 15, YELLOW);  // M1 (simple rectangle)

  // See attached docs for math calculations for implementations below
  // Triangles for motor 3 (no inbuilt slanted rectangle function)
  tft.fillTriangle( // M3-1 (filling)
    28.29,  39,     // Point D
    8.29,   73.64,  // Point A
    21.29,  81.14,  // Point E
    BLACK);
  tft.drawTriangle( // M3-1
    28.29,  39,     // Point D
    8.29,   73.64,  // Point A
    21.29,  81.14,  // Point E
    YELLOW);
    
  tft.fillTriangle( // M3-2 (filling)
    28.29,  39,     // Point D
    21.29,  81.14,  // Point E
    41.29,  46.5,   // Point B
    BLACK);
  tft.drawTriangle( // M3-2
    28.29,  39, // Point D
    21.29,  81.14,  // Point E
    41.29,  46.5,    // Point B
    YELLOW);
   tft.drawLine(21.29, 81.14, 28.29, 39 , BLACK);// Cover leftover line from triangles drawn

  // Triangles for motor 2 (no inbuilt slanted rectangle function)
  tft.fillTriangle( // M2-1 (Filling)
    129.71, 81.14,  // Point D
    109.71, 46.5,   // Point A
    122.71, 39,     // Point E
    BLACK);
  tft.drawTriangle( // M2-1
    129.71, 81.14,  // Point D
    109.71, 46.5,   // Point A
    122.71, 39,     // Point E
    YELLOW);
    
  tft.fillTriangle( // M2-2 (Filling)
    129.71, 81.14,  // Point D
    122.71, 39,     // Point E
    142.71, 73.64,  // Point B
    BLACK);
  tft.drawTriangle( // M2-2
    129.71, 81.14,  // Point D
    122.71, 39,     // Point E
    142.71, 73.64,  // Point B
    YELLOW);
  tft.drawLine(122.71, 39, 129.71, 81.14, BLACK); // Cover leftover line from triangles drawn
  
}


// Function to measure then display battery levels on menubar (Do not call function too quickly)
void drawBatteryIcon(){
  float battLevel = getcurrVcc();
  float battPercent;
  int LEVELCOLOUR;
  if (battLevel < 5400){ // If battery is less than 5.4v, likely connected to USB supply, not battery
    tft.setCursor(296, 4);
    tft.setTextColor(GREEN);  tft.setTextSize(1);
    tft.print("PWR"); // Write inside battery instead of filling
    
    tft.drawRect(292, 2, 25, 11, GREEN);  // Draw battery
    tft.drawRect(289, 4, 4, 6, GREEN);    // Draw battery nib
    
  }else{  // Battery is connected to VCC in
    battPercent = ((battLevel - lowVolt)/(highVolt-lowVolt))*100; // Determine battery percentage based on expected voltages
    
    // Determine colour for battery level
    if (battPercent > 75) LEVELCOLOUR = GREEN;
    else if (battPercent > 50) LEVELCOLOUR = ORANGE;
    else if (battPercent > 25) LEVELCOLOUR = YELLOW;
    else LEVELCOLOUR = RED;

    //Draw icon fill  
    int fillAmount; // Holds length of filled in bar (pixels)
    if (battLevel > highVolt) fillAmount = 23;  // Overflow high
    else if (battLevel < highVolt) fillAmount = 0;  // Overflow low
    else fillAmount = 0.23*battPercent;  // Find length of battery bar based on percentage
    tft.fillRect(316 - fillAmount, 3, fillAmount, 9, LEVELCOLOUR);
    
    // Draw icon outline
    if (battPercent > 95) tft.fillRect(289, 4, 4, 6, LEVELCOLOUR);  // Fill battery nib if full battery
    if (fillAmount > 0){  // Bar is visible on screen
      tft.drawRect(292, 2, 25, 11, WHITE);    // Draw plain battery
      tft.drawRect(289, 4, 4, 6, WHITE);      // Draw plain battery nib
    }else{  // battery is critical (bar not visible)
      tft.drawRect(292, 2, 25, 11, RED);    // Draw critical battery
      tft.drawRect(289, 4, 4, 6, RED);      // Draw critical battery nib
      tft.setCursor(296, 4);
      tft.setTextColor(RED);  tft.setTextSize(1);
      tft.print("LOW"); // Write inside battery in place of fill
    }
    
  }
}


// Voltage determination function (By RetroLefty - Arduino Forums)
int getcurrVcc(void){ // Returns actual value of Vcc (x 100) Volts
    // For 168/328 boards
    const long InternalReferenceVoltage = 1056L;            // Adjust this value to your boards specific internal BG voltage x1000
       // REFS1 REFS0          --> 0 1, AVcc internal ref.  -Selects AVcc external reference
       // MUX3 MUX2 MUX1 MUX0  --> 1110 1.1V (VBG)          -Selects channel 14, bandgap voltage, to measure
    ADMUX = (0<<REFS1) | (1<<REFS0) | (0<<ADLAR) | (1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (0<<MUX0);
     
    delay(0.5);
       // Start a conversion  
    ADCSRA |= _BV( ADSC );
       // Wait for it to complete
    while( ( (ADCSRA & (1<<ADSC)) != 0 ) );
       // Scale the value
    int results = (((InternalReferenceVoltage * 1024L) / ADC) + 5L) / 10L; // calculates for straight line value
    results = results*10;
    return results;
   }
