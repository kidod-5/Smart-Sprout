#include <SPI.h>
#include "Mirf.h"
#include "nRF24L01.h"
#include "MirfHardwareSpiDriver.h"
#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>

const int waterLevelPin = A0; 
const int moisturePin = A1; 
uint16_t dataPackage[3]; 

LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS3231 rtc;

void setup(){
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  rtc.begin();
  
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"SEND1");
  Mirf.payload = sizeof(dataPackage);
  Mirf.channel = 90; 
  Mirf.config();
  
  Serial.println("--- System Initialized ---");
}

void loop(){
  DateTime now = rtc.now();
  
  // 1. Read Sensors
  int rawMoisture = analogRead(moisturePin);
  int rawWater = analogRead(waterLevelPin);
  
  dataPackage[0] = rawMoisture;
  dataPackage[1] = rawWater;

  // 2. Logic (Using your test values)
  // Only pump if Soil is dry (<300) AND Tank is NOT empty (>240)
  if(rawMoisture < 300 && rawWater >= 240) {
    dataPackage[2] = 1; // PUMP ON
  } else {
    dataPackage[2] = 0; // PUMP OFF
  }

  // 3. Serial Monitor 
  Serial.print("["); Serial.print(now.hour()); Serial.print(":"); Serial.print(now.minute()); Serial.print("] ");
  Serial.print("M:"); Serial.print(rawMoisture);
  Serial.print(" | W:"); Serial.print(rawWater);
  
  if (rawWater < 240) Serial.println(" - TANK EMPTY");
  else if (rawWater < 500) Serial.println(" - WATER LOW");
  else Serial.println(" - TANK FULL");

  // 4. LCD Update
  lcd.setCursor(0,0);
  lcd.print("T:"); lcd.print(now.hour()); lcd.print(":"); lcd.print(now.minute());
  lcd.print(" Tank:"); 
  if(rawWater < 240) lcd.print("DRY");
  else lcd.print("OK ");

  lcd.setCursor(0,1);
  lcd.print("M:"); lcd.print(rawMoisture);
  lcd.print(dataPackage[2] == 1 ? " *PUMPING*" : "  Waiting");

  // 5. Send to others
  Mirf.setTADDR((byte *)"RECVR");
  Mirf.send((byte *) &dataPackage);
  while(Mirf.isSending()){}

  delay(1000); 
}
