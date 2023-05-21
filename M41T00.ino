//////////////////////////////////////////////
// What's the time?                         //
// Let's set!                               //
// This is an old and easy code but useful. //
// Created by Pungi                         //
//////////////////////////////////////////////

// Include the wire lib 
#include "Wire.h"

// define the address of the RTC chip
#define M41T00_I2C_ADDRESS 0x68

// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return ( (val/10*16) + (val%10) );
}

// Stops the M41T00, but it has the side effect of setting seconds to 0
// Probably only want to use this for testing
/*
void stopM41T00()
{
  Wire.beginTransmission(M41T00_I2C_ADDRESS);
  Wire.write(0);
  Wire.write(0x80);
  Wire.endTransmission();
}
*/

// 1) Sets the date and time on the M41T00
// 2) Starts the clock
// 3) Sets hour mode to 24 hour clock
// Assumes you're passing in valid numbers
// second        // 0-59
// minute        // 0-59
// hour          // 1-23
// dayOfWeek     // 1-7
// dayOfMonth    // 1-28/29/30/31
// month         // 1-12
// year          // 0-99
void set_M41T00(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)
{
   Wire.beginTransmission(M41T00_I2C_ADDRESS);
   Wire.write(0);
   Wire.write(decToBcd(second));    // 0 to bit 7 starts the clock
   Wire.write(decToBcd(minute));
   Wire.write(decToBcd(hour));      // If you want 12 hour am/pm you need to set
                                   // bit 6 (also need to change readDateM41T00)
   Wire.write(decToBcd(dayOfWeek));
   Wire.write(decToBcd(dayOfMonth));
   Wire.write(decToBcd(month));
   Wire.write(decToBcd(year));
   Wire.endTransmission();
}

void setup()
{
  // Create values
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // Init the wire
  Wire.begin();
  // Change these values to what you want to set your clock to.
  // You probably only want to set your clock once and then remove
  // the set_M41T00 call.
  second = 30;
  minute = 41;
  hour = 18;
  dayOfWeek = 3;
  dayOfMonth = 23;
  month = 4;
  year = 14;
  // Send the values to the chip
  set_M41T00(second, minute, hour, dayOfWeek, dayOfMonth, month, year);
}

void loop()
{
}
