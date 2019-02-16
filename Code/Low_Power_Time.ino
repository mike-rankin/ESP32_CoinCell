/*
The ESP32 Coin Cell Board uses Tools/Board/ESP32 Dev Module
Simple Deep Sleep with Timer Wake Up Displaying Time every 10 minutes
=====================================

This code is under Public Domain License.

Original Author:
Pranav Cherukupalli <cherukupallip@gmail.com>
*/

#include <WiFi.h>
#include <Wire.h>
#include "time.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>

#define uS_TO_S_FACTOR 1000000   /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  60        /* Time ESP32 will go to sleep (in seconds) 600 = 10 minutes */  
#define OLED_RESET 26             /*ESP32 Pico D4 */

#define RED_LED 23  //lights Green
#define GREEN_LED 18  //lights Red
#define BLUE_LED 5

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    96

Adafruit_SSD1306 display(96, 16, &Wire, 26);

const char* ssid       = "Turd";
const char* password   = "catinthehat";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -14400;
const int   daylightOffset_sec = 3600;

RTC_DATA_ATTR int bootCount = 0;


struct tm timeinfo;

void setup()
{
  Serial.begin(9600);
  delay(1000); 

  pinMode(OLED_RESET, OUTPUT);
 
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
 
  digitalWrite(OLED_RESET, HIGH);   //Keep Oled High = 0.21mA, OLED_RESET LOW draws lots of current

  digitalWrite(RED_LED, HIGH);   //Keep Led off
  digitalWrite(GREEN_LED, HIGH); //Keep Led off
  digitalWrite(BLUE_LED, HIGH);  //Keep Led off

  Wire.begin(13,14);  //ESP32 Pico D4 pins
  display.setRotation(2);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 

   //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  digitalWrite(GREEN_LED, LOW);
  delay(150);
  digitalWrite(GREEN_LED, HIGH);

  //display.display();
  delay(2000);
  display.clearDisplay();

  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) +
  " Seconds");

 
 
}

void loop()
{
   struct tm timeinfo;
  if(!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }

  digitalWrite(RED_LED, LOW);  //LED on
  delay(100);
  digitalWrite(RED_LED, HIGH);
  delay(100);

  digitalWrite(GREEN_LED, LOW);  //LED on
  delay(100);
  digitalWrite(GREEN_LED, HIGH);
  delay(100);

  digitalWrite(BLUE_LED, LOW);
  delay(100);
  digitalWrite(BLUE_LED, HIGH);
  delay(100);

  display.ssd1306_command(SSD1306_DISPLAYON);
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);  //over,down
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  display.println(&timeinfo, "%H:%M:%S");
  display.display();
  delay(1000);
  display.clearDisplay();

  display.ssd1306_command(SSD1306_DISPLAYOFF);

  Serial.println("Going to sleep now");
  esp_deep_sleep_start();
}
