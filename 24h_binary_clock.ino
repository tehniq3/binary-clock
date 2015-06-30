// http://www.multiwingspan.co.uk/arduino.php?page=bclock
// adapted sketch for 24 hour format by niq_ro from http://arduinotehniq.blogspot.com/
// 30.06.2015 - Craiova

#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 RTC;
int datapin = 2;      // DATA for IC1
int clockpin = 3;     // CLK for IC1
int latchpin = 4;     // LATCH for IC1
int datapin2 = 8;     // DATA for IC2
int clockpin2 = 9;    // CLK for IC2
int latchpin2 = 10;   // LATCH for IC2
int h24 = 11;         // extra led for 24 hour format

void setup()
{
  Serial.begin(57600);
  Wire.begin();
  RTC.begin();

  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    //RTC.adjust(DateTime(__DATE__, __TIME__));
  }

  pinMode(datapin, OUTPUT);
  pinMode(clockpin, OUTPUT); 
  pinMode(latchpin, OUTPUT);
  pinMode(datapin2, OUTPUT);
  pinMode(clockpin2, OUTPUT); 
  pinMode(latchpin2, OUTPUT);
  pinMode(h24, OUTPUT);
}

void loop()
{

  DateTime now = RTC.now();
  // All used for checking the time of the clock
  // This section can be removed when everything is working
  Serial.print(now.day(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.year(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
  // End of section that can be removed

  int mins = now.minute();
  int secs = now.second();
  int hr = now.hour();
/*
  // convert to 12 hour time
  if (hr>12)
  {
    hr = hr-12; 
  }
*/

  // variables to describe pattern of on lights
  byte data1 = 0;
  byte data2 = 0;


  // encode the time

if (bitRead(hr,4)==1) digitalWrite(h24,HIGH);
else digitalWrite(h24,LOW);

  // hr = 1st four bits
  for (int i =0;i<4;i++)
  {
    if (bitRead(hr,i)==1) 
    {
      bitWrite(data1,3-i,1); 
    }
  }
  // mins on the first shift register
  for (int i =2;i<6;i++)
  {
    if (bitRead(mins,i)==1)
    {
      bitWrite(data1,9-i,1); 
    }
  }
  // remaining mins LEDs
  for (int i =0;i<2;i++)
  {
    if (bitRead(mins,i)==1)
    {
      bitWrite(data2,1-i,1); 
    }
  }
  // seconds
  for (int i =2;i<8;i++)
  {
    if (bitRead(secs,i-2)==1)
    {
      bitWrite(data2,9-i,1); 
    }
  }
  // output the information
  writeByte(data1,1);
  writeByte(data2,2);
  // a brief pause
  delay(500);

}

void writeByte(byte data, byte set)
{
  int d,c,l;
  if (set==1)
  {
    d = 2;
    c = 3;
    l = 4;
  }
  else if (set==2)
  {
    d = 8;
    c = 9;
    l = 10;
  }
  shiftOut(d, c, MSBFIRST, data);
  // toggle the latch pin so that the data appears as an output
  digitalWrite(l, HIGH);
  digitalWrite(l, LOW);
}
