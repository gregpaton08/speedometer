/*! \file speedometer.ino
 *  \brief Arduino sketch for monitoring speed
 *         usind OBD and OLED display.
 *  \author Greg Paton
 *  \date 16 May 2013
 */
// Copyright (c) GSP 2013


// D0  - 10
// D1  -  9
// DC  - 11
// RST - 13
// CD  - 12


#include "OBDLib.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_DC 11
#define OLED_CS 12
#define OLED_CLK 10
#define OLED_MOSI 9
#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

OBDLib obd;


void setup() {
  // set BAUD rate
  Serial.begin(38400);
  
  // Setup display 
  display.begin();
  display.clearDisplay();
  
  // display "connect OBD"
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("connect");
  display.println("OBD");
  display.display();
  
  // Setup OBD
  obd.init(); 
  
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("OBD");
  display.println("connected");
  display.display();
}


void loop() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(4);
  String str = getSpeed();
   
  if (str.length() == 1)
    display.print(" ");
  display.println(str);
  display.setTextSize(2);
  display.println("MPH");
  
  display.display();
}

String getSpeed() {
  uint8_t len = 0;
  uint8_t pidResSize = 3;
  char pidRes[pidResSize];
  
  // Query PID
  obd.sendCMD(0x01, 0x0D);
  
  while (true) {
    if (Serial.find("0D"))
      break;
    if (Serial.find("UNABLE TO CONNECT")) 
      return String("UTC");
  }
   
  // Read next 2 characters
  while (len < pidResSize - 1) {
    unsigned char c = Serial.read();
    if (c == (unsigned char)-1 || c == 32) continue;
    if (c == '\n' || c == '\r' || c < 48) break;
    pidRes[len] = c;
    ++len;
  }
  pidRes[pidResSize - 1] = '\0';
  
  return String((int)(obd.pidToDec(0x0D, pidRes) + 0.5));
}
