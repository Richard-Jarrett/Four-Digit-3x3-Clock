// Richard Jarrett 
// 0.1 Based on RTCLib/ds1307

// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"
#include <FastLED.h>

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif

// WS2812B Setup
// FastLED setup
#define NUM_LEDS 36
#define DATA_PIN 2
CRGB leds[NUM_LEDS];

// Setup variables for grid size (3 x 3)
int gridSize = 9;

// Setup variables for HSV increment
int incrementHSVSmall = 5;
int incrementHSVLarge = 50;

// Setup Variables for time
int currentHoursTens = 0;
int currentHoursUnits = 0;
int currentMinutesTens = 0;
int currentMinutesUnits = 0;
int currentSecondsTens = 0;
int currentSecondsUnits = 0;

// Setup variables for LED drive in the HSV space
int currentH = 0;
int currentS = 255;
int currentV = 127;

// RTC Setup
RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup () {

  // FastLED Setup
  delay(2000);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);

  Serial.begin(9600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
}

void loop () {
  // Get current time and date from RTC
  DateTime now = rtc.now();

  /*
  // Serial print of digits from RTC, and extracted digits
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
  // Dumb Serial Print- Time extracted from RTC seperate pieces- used for Debug
  Serial.print("Extracted Time:");
  Serial.println();
  Serial.print(currentHoursTens, DEC);
  Serial.print("_");
  Serial.print(currentHoursUnits, DEC);
  Serial.print(":");
  Serial.print(currentMinutesTens, DEC);
  Serial.print("_");
  Serial.print(currentMinutesUnits, DEC);
  Serial.print(":");
  Serial.print(currentSecondsTens, DEC);
  Serial.print("_");
  Serial.print(currentSecondsUnits, DEC);
  Serial.println();
  Serial.print("--------");
  Serial.println();  
  Serial.println();
  */

  // Seperate time out into seperate Tens and Units for each
  currentHoursTens = (now.hour() / 10);
  currentHoursUnits = (now.hour() % 10);
  currentMinutesTens = (now.minute() / 10);
  currentMinutesUnits = (now.minute() % 10); 
  currentSecondsTens = (now.second() / 10);
  currentSecondsUnits = (now.second() % 10);

  // Update the display
  FastLED.show();
  delay(50);

  // Clear the display- Shows Update
  // displayBlank();
  // delay(1);

  // Display the time
  // Tens Hours
  displayNumber(1, currentHoursTens);
  currentH = (currentH + incrementHSVLarge);
  // Units Hours
  displayNumber(2, currentHoursUnits);
  currentH = (currentH + incrementHSVLarge);
  // Tens Minutes
  displayNumber(3, currentMinutesTens);
  currentH = (currentH + incrementHSVLarge);
  // Units Hours
  displayNumber(4, currentMinutesUnits);
  currentH = (currentH + incrementHSVLarge);
  // 
}

  // Function that splits the current time into seperate sections
  

  // Function that takes a number and lights that number of LED's, starting with 0
  // Improvements- Show at the exit of the loop, Variable for corner or zero LED
void displayNumber(int digit, int value){
  
  int offset = ((digit - 1) * 9);
  
  // Flashing LED in centre for zero
  if(value == 0) {
    displayBlankDigit(digit);
    leds[(4 + offset)] = CRGB::White;
    leds[(4 + offset)].fadeLightBy(250);
    FastLED.show();
  }

  // Displaying digit in LED's
  for(int thisLED = (value + offset); thisLED != offset; thisLED = thisLED -1) {
    leds[thisLED - 1] = CHSV((currentH % 256), currentS, currentV);
  /*
    // Print CHSV colour information for each set LED
    Serial.print("LED :");
    Serial.print(thisLED);
    Serial.print(" CHSV :");
    Serial.print((currentH % 256));
    Serial.print(", ");
    Serial.print(currentS);
    Serial.print(", ");
    Serial.print(currentV);
    Serial.println();
  */
    // Little shift in HSV
    currentH = currentH + incrementHSVSmall;
    FastLED.show();
    // delay(10);
  }
}

  // Function that clears down each LED sequentially to black
  // Improvements- show LEDs at the end of the loop
void displayBlank(){
  for(int thisLED = 0; thisLED < NUM_LEDS; thisLED = thisLED +1) {
  leds[thisLED] = CRGB::Black;
  FastLED.show();
  //delay(100);    
  }
}
void displayBlankDigit(int digit){
  int offsetStop = ((digit - 1) * 9);
  int offsetStart = (digit * 9);
  for(int thisLED = (offsetStart); thisLED != offsetStop; thisLED = thisLED -1) {
  leds[thisLED] = CRGB::Black;
  }
}    
