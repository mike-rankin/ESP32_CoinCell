/*********************************************************************
 Thanks to Adafruit for the great oled display library. It's the best one for a 96x16 type
 Install the GFX, SSD1306 and LIS3DH libraries from the IDE
 Install the Adafruit Sensor Library from: https://github.com/adafruit/Adafruit_Sensor
 *********************************************************************/


#include <WiFi.h>
#include <Wire.h>
#include "time.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>

//I've messed up the led names and see they do not match the schematic
#define BLUE_LED 23  
#define RED_LED 5  
#define GREEN_LED 18

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    96

//Doesnt work
static const unsigned char PROGMEM logo_bmp[] =
{
};


Adafruit_SSD1306 display(96, 16, &Wire, 26);
Adafruit_LIS3DH lis = Adafruit_LIS3DH();

const char* ssid       = "SSID";
const char* password   = "PASSWORD";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -14400;
const int   daylightOffset_sec = 3600;
RTC_DATA_ATTR int bootCount = 0;

struct tm timeinfo;

void setup()
{
  Serial.begin(9600);
  delay(1000);

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  digitalWrite(RED_LED, HIGH);   //Keep off
  digitalWrite(GREEN_LED, HIGH); //Keep off
  digitalWrite(BLUE_LED, HIGH);  //Keep off

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

  display.display();
  delay(2000);
  display.clearDisplay();
  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  Serial.println("LIS3DH test!");
  
  if (! lis.begin(0x18)) {   // change this to 0x19 for alternative i2c address
    Serial.println("Couldnt start");
    while (1);
  }
  Serial.println("LIS3DH found!");
  
  lis.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G!
  Serial.print("Range = "); Serial.print(2 << lis.getRange());  
  Serial.println("G");
 
}

void loop()
{
  //testdrawbitmap();  //No worky

  if (touchRead(T0) < 50)
  {
  digitalWrite(BLUE_LED, LOW);
  }
  else
  {
  digitalWrite(BLUE_LED, HIGH);
  }

  lis.read(); 
  sensors_event_t event; 
  lis.getEvent(&event);
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,8);  //over,down
  display.print(event.acceleration.x);
  display.print(" ");
  display.print(event.acceleration.y);
   display.print(" ");
  display.print(event.acceleration.z);

  if(!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);  //over,down
  display.print(&timeinfo, "%H:%M:%S");
  display.display();
  display.clearDisplay();
}

//Doesnt Work
void testdrawbitmap(void) {
  display.clearDisplay();

  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(1000);
}
