//////////////////////////////////////////////
// What's the time?                         //
//                                          //
// This is an old and easy code but useful. //
// Created by Pungi                         //
//////////////////////////////////////////////

// Include some libs.. 
#include "Wire.h"
#include <LiquidCrystal.h>

// define the address of the RTC chip
#define M41T00_I2C_ADDRESS 0x68

// The arduino pin numbers and the LCD pins
// LCD pins: 12 = RS, 11 = E, 5 = D4 , 4 = D5 , 3 = D6 , 2 = D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);      // LCD pin connections and it's initializing

// HD44780 LCD special character
// Let's create the byte array of the letter ó
byte hosszuO[8] = {
  0b00010,
  0b00100,
  0b01110,
  0b10001,
  0b10001,
  0b10001,
  0b01110,
  0b00000,
};

// HD44780 LCD special character
// Let's create the byte array of the letter é
byte hosszuE[8] = {
  0b00010,
  0b00100,
  0b01110,
  0b10001,
  0b11111,
  0b10000,
  0b01110,
  0b00000
};

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return ( (val/16*10) + (val%16) );
}

// Gets the values from the M41T00
void get_M41T00(byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *dayOfMonth, byte *month, byte *year)
{
  // Reset the register pointer
  Wire.beginTransmission(M41T00_I2C_ADDRESS);
  Wire.write(0);
  Wire.endTransmission();
  
  Wire.requestFrom(M41T00_I2C_ADDRESS, 7);

  // A few of these need masks because certain bits are control bits
  *second     = bcdToDec(Wire.read() & 0x7f);
  *minute     = bcdToDec(Wire.read());
  *hour       = bcdToDec(Wire.read() & 0x3f);  // Need to change this if 12 hour am/pm
  *dayOfWeek  = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month      = bcdToDec(Wire.read());
  *year       = bcdToDec(Wire.read());
}

void setup()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  Wire.begin();
  Serial.begin(9600);
  lcd.begin(20, 4);
  lcd.createChar(0, hosszuO);
  lcd.createChar(1, hosszuE);
}

void loop()
{
  // Create byte values
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // Read the values of the I2C clock chip
  get_M41T00(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  // Set the cursor. First position, first line.
  lcd.setCursor(0, 0);
  // Write the text. "Óra"
  lcd.write(byte(0));
  lcd.print("ra!");
  // Set the cursor. Sixth position, second line.
  lcd.setCursor(6, 1);
  // If the number is less 10, write one space.
  if(!(hour>9))
  {
    lcd.print(" ");
  };
  lcd.print(hour, DEC);
  lcd.print(":");
  // Set the cursor. Ninth position, second line.
  lcd.setCursor(9, 1);
  //  If the number is less 10, write one space.
  if(!(minute>9))
  {
    lcd.print(" ");
  };
  lcd.print(minute, DEC);
  lcd.print(":");
  // Set the cursor. Twelfth position, second line.
  lcd.setCursor(12, 1);
  // If the number is less 10, write one space.
  if (second<10)
  {
    lcd.print("0");
  }; 
  lcd.print(second, DEC);
  // Set the cursor. First position, third line.
  lcd.setCursor(0, 2);
  lcd.print(2000+year, DEC);
  lcd.print(".");
  //  If the number is less 10, write one space.
  if(!(month>9))
  {
    lcd.print(" ");
  };
  lcd.print(month, DEC);
  lcd.print(".");
  //  If the number is less 10, write one space.
  if(!(dayOfMonth>9))
  {
    lcd.print(" ");
  };
  lcd.print(dayOfMonth, DEC);
  // Set the cursor. First position, third line.
  lcd.setCursor(0, 3);
  // Write the text. "Hét napja: "
  lcd.print("H");
  lcd.write(byte(1));
  lcd.print("t napja: ");
  lcd.print(dayOfWeek, DEC);

  // Serial sending
  Serial.print(hour, DEC);
  Serial.print(":");
  Serial.print(minute, DEC);
  Serial.print(":");
  Serial.print(second, DEC);
  Serial.print("  ");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(year, DEC);
  // Send "day of week" in hungarian. "Hét napja"
  Serial.print("  H");
  Serial.write(0xE9); //  é
  Serial.print("t napja:");
  Serial.println(dayOfWeek, DEC);
  // Write my nick name.
  Serial.print("Pungi");
  Serial.write(0xAE);
  Serial.println("");
  delay(1000);
}
