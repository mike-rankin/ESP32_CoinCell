/*
The ESP32 Coin Cell Board uses Tools/Board/ESP32 Dev Module
Simple Deep Sleep with Timer Wake Up Displaying Time every 15 minutes
Thanks to Adafruit and Espressif for the library filed
Boards: ESP32 Dev Module
*/

#include <WiFi.h>
#include <Wire.h>
#include "time.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
 
#define OLED_RESET 26    

#define Threshold 40 

#define BLUE_LED 23  
#define RED_LED 5  
#define GREEN_LED 18

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    96  

static const unsigned char PROGMEM logo_bmp[] =
{ 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x03, 0xf3, 0xff, 0x18, 0xe0, 0x1e, 0x01, 0x80, 0x1e, 0x03, 0xb8, 0x00, 
    0x01, 0x94, 0x59, 0xac, 0xb0, 0x32, 0x00, 0x00, 0x32, 0x01, 0x98, 0x00, 
    0x01, 0x87, 0x19, 0x8c, 0x30, 0x60, 0x33, 0xb4, 0x60, 0x31, 0x98, 0x00, 
    0x01, 0xe3, 0xdf, 0x18, 0x60, 0x60, 0x69, 0x9a, 0x60, 0x69, 0x98, 0x00, 
    0x01, 0x80, 0xd8, 0x0c, 0x40, 0x60, 0x69, 0x9a, 0x60, 0x79, 0x98, 0x00, 
    0x01, 0x94, 0x58, 0x2c, 0x90, 0x26, 0x69, 0x9a, 0x26, 0x61, 0x98, 0x00, 
    0x03, 0xf7, 0xbc, 0x18, 0xf0, 0x1c, 0x33, 0xfb, 0x1c, 0x3b, 0xfc, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
   
};
 
Adafruit_SSD1306 display(96, 16, &Wire, 26);
//touch_pad_t touchPin;

const char* ssid       = "SSID";
const char* password   = "PASSWORD";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -14400;
const int   daylightOffset_sec = 3600;

RTC_DATA_ATTR int bootCount = 0;

struct tm timeinfo;

void callback()
{
  //placeholder callback function
}

void setup()
{
  Serial.begin(9600);
  delay(1000); 

  pinMode(OLED_RESET, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  
  digitalWrite(OLED_RESET, HIGH);   //Keep Oled High = 0.21mA, OLED_RESET LOW draws lots of current
  digitalWrite(RED_LED, HIGH);   //Keep off
  digitalWrite(GREEN_LED, HIGH); //Keep off
  digitalWrite(BLUE_LED, HIGH);  //Keep off

  blinkRGB();

  Wire.begin(13,14);  
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
  
  //testdrawbitmap();

  delay(2000);
  display.clearDisplay();
  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);   //init and get the time

  touchAttachInterrupt(T0, callback, Threshold);
  esp_sleep_enable_touchpad_wakeup();   //Configure Touchpad as wakeup source
}

void loop()
{
  if(!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  blinkRGB();
  display.ssd1306_command(SSD1306_DISPLAYON);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);  //over,down
  display.print(&timeinfo, "%H:%M:%S");
  display.display();
  //delay(2000);
  display.clearDisplay();
  display.ssd1306_command(SSD1306_DISPLAYOFF);
 
  Serial.println("Going to sleep now");
  esp_deep_sleep_start();
}


void testdrawbitmap(void)
{
  display.clearDisplay();

  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(2000);
  display.clearDisplay();
}

void blinkRGB(void)
{
  digitalWrite(RED_LED, LOW);   //On
  delay(50);
  digitalWrite(RED_LED, HIGH);   //Off
  delay(50);
  digitalWrite(GREEN_LED, LOW); 
  delay(50);
  digitalWrite(GREEN_LED, HIGH);
  delay(50);
  digitalWrite(BLUE_LED, LOW); 
  delay(50);
  digitalWrite(BLUE_LED, HIGH);
}
