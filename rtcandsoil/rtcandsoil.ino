#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS3231 rtc;

// Pins
const int moisturePin = A1;   // analog output from moisture sensor
const int pumpPin = 7;        // relay or transistor control pin for pump
const int redStatusLED = 5;
const int yellowLED = 6;    // Warning LED
const int greenLED = 10;     // good LED

// Moisture threshold 
const int dryThreshold = 300;  // starting point, adjust after testing
const int warningThreshold = 500; // yellow light starts here

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  pinMode(pumpPin, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(redStatusLED, OUTPUT);

  digitalWrite(pumpPin, LOW);
 

  if (!rtc.begin()) {
    Serial.println("RTC not found");
    lcd.setCursor(0, 0);
    lcd.print("RTC not found");
    while (1);
  }

  // Use this once if the RTC needs to be set, then comment it out
  //rtc.adjust(DateTime(2026, 4, 26, 10, 28, 30));

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Plant System");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");
  delay(2000);
  lcd.clear();
}

void loop() {
  //current time
  DateTime now = rtc.now();

  // soil moisture
  int moistureValue = analogRead(moisturePin);

  lcd.setCursor(0, 1); 
  lcd.print("M:");
  lcd.print(moistureValue);
  lcd.print(" ");

  if (moistureValue < 300) {
    digitalWrite(redStatusLED, HIGH); 
    digitalWrite(pumpPin, HIGH);      
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, LOW);
    lcd.print("PUMPING   "); // Prints after the moisture value
  } 
  else if (moistureValue <= 500) {
    digitalWrite(redStatusLED, LOW); 
    digitalWrite(pumpPin, LOW);       
    digitalWrite(yellowLED, HIGH); 
    digitalWrite(greenLED, LOW);
    lcd.print("WARNING   ");
  } 
  else {
    digitalWrite(redStatusLED, LOW);
    digitalWrite(pumpPin, LOW);       
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, HIGH);
    lcd.print("MOIST     ");
  }

  // ----- LCD LINE 1: TIME -----
  lcd.setCursor(0, 0);

  int hour12 = now.hour();
  bool isPM = false;

  if (hour12 >= 12) isPM = true;
  if (hour12 == 0) hour12 = 12;
  else if (hour12 > 12) hour12 -= 12;

  if (hour12 < 10) lcd.print("0");
  lcd.print(hour12);
  lcd.print(":");

  if (now.minute() < 10) lcd.print("0");
  lcd.print(now.minute());
  lcd.print(" ");

  if (isPM) lcd.print("PM");
  else lcd.print("AM");

  lcd.print("   ");

  // LCD: MOISTURE / STATUS
  lcd.setCursor(0, 1);

  lcd.print("M:");
  lcd.print(moistureValue);
  lcd.print(" ");


  // Serial monitor for debugging
  Serial.print("Time: ");
  Serial.print(now.hour());
  Serial.print(":");
  Serial.print(now.minute());
  Serial.print(":");
  Serial.print(now.second());
  Serial.print(" | Moisture: ");
  Serial.print(moistureValue);
  Serial.print(" | Pump: ");

  delay(1000);
}
