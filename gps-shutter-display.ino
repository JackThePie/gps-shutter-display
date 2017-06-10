/* Code for Arduino based system. 
Every set time GPS module is gathering data 
and shutter is triggering photo.
Display is showing status.
by Jacek Bera */


#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Delay in ms, 1000ms=1s=1Hz
int x = 1000; 								

// Set address of display on 0x27
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  

const int chipSelect = 10;

static const int RXPin = 8, TXPin = 7;
static const uint32_t GPSBaud = 9600;

// TinyGPS++ object
TinyGPSPlus gps;

// Serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

Servo myservo;
int pos = 0;

void setup()
{

  myservo.attach(9);
  Serial.begin(115200);
  ss.begin(GPSBaud);
  if (!SD.begin(chipSelect)) {
    return;
  } 
  
}
void loop()
{
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("satelitow: ");
  lcd.print(gps.satellites.value());
  lcd.setCursor(0, 1);
  lcd.print("interwal: ");
  lcd.print(x);
  lcd.print("ms");

// Trigger goes from 0 degrees to 180 degrees
  for (pos = 0; pos <= 360; pos += 40) { 
// Tell servo to go to position in variable 'pos'
    myservo.write(pos);   
// Waits 25ms for the servo to reach the position	
    smartDelay(25);                       
  }
// Goes from 180 degrees to 0 degrees
  for (pos = 360; pos >= 0; pos -= 10) { 
// Tell servo to go to position in variable 'pos'
    myservo.write(pos);  
// Waits 15ms for the servo to reach the position	
    smartDelay(25);                       
  }
  
      File dataFile = SD.open("datalog.txt", FILE_WRITE);
      if (dataFile) {
        dataFile.print(gps.satellites.value());
        dataFile.print("; ");
        dataFile.print(gps.hdop.value());
        dataFile.print("; ");
        dataFile.print(gps.location.lat(), 6);
        dataFile.print("; ");
        dataFile.print(gps.location.lng(), 6);
        dataFile.print("; ");
        dataFile.print(gps.location.age());
        dataFile.print("; ");
        dataFile.print(gps.altitude.meters());
        dataFile.print("; ");
        dataFile.print(gps.course.deg());
        dataFile.print("; ");
        dataFile.print(gps.speed.kmph());
        dataFile.print("; ");
        dataFile.print(gps.time.value());
        dataFile.print("; ");      
        dataFile.println();
        dataFile.close();
      }
    }

void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (ss.available())
      gps.encode(ss.read());
  } 
  while (millis() - start < ms);
}
