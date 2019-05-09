 /*********************************************************************
 The ESP32 Coin Cell Board uses Tools/Board/ESP32 Dev Module

 Thanks to Adafruit for the great oled display library. It's the best one for a 96x16 type
 Install the GFX, SSD1306, LIS3DH and ClosedCube HDC1080, libraries from the IDE
 Install the Adafruit Sensor Library from: https://github.com/adafruit/Adafruit_Sensor
 *********************************************************************/

 #include <Wire.h>
 #include <WiFi.h>
 #include "time.h"
 #include <Adafruit_GFX.h>
 #include <Adafruit_SSD1306.h>
 #include <Adafruit_LIS3DH.h>
 #include <Adafruit_Sensor.h>
 #include "ClosedCube_HDC1080.h"

 Adafruit_SSD1306 display(96, 16, &Wire, 26);
 Adafruit_LIS3DH lis = Adafruit_LIS3DH();
 ClosedCube_HDC1080 hdc1080;

 #define ACCEL_PWR 34
 #define OLED_RESET 26
 #define BLUE_LED 23  
 #define RED_LED 5  
 #define GREEN_LED 18
 #define LOGO16_GLCD_HEIGHT 16
 #define LOGO16_GLCD_WIDTH  96  //was 16

 const char* ssid       = "SSID";  
 const char* password   = "PASSWORD";  

 int threshold = 40;
 bool touch1detected = false;
 bool touch2detected = false;
 int wifi_connected;

 void gotTouch1()
 {
 touch1detected = true;
 }

void gotTouch2(){
 touch2detected = true;
}

void callback()
{
 //placeholder callback function
}

void setup() 
{
 Serial.begin(115200);
 delay(1000); // give me time to bring up serial monitor
 
 pinMode(ACCEL_PWR, OUTPUT);
 
 pinMode(OLED_RESET, OUTPUT);
 pinMode(RED_LED, OUTPUT);
 pinMode(GREEN_LED, OUTPUT);
 pinMode(BLUE_LED, OUTPUT);
 
 digitalWrite(ACCEL_PWR, HIGH);
 digitalWrite(OLED_RESET, HIGH);
 digitalWrite(RED_LED, HIGH);   //Keep off
 digitalWrite(GREEN_LED, HIGH); //Keep off
 digitalWrite(BLUE_LED, HIGH);  //Keep off

 Wire.begin(13,14);             //ESP32 i2c pins
 display.setRotation(2);
 display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
 hdc1080.begin(0x40);
 
 blinkRGB();

 display.clearDisplay();  

 //Now connect to WiFi
 Serial.printf("Connecting to %s ", ssid);
 WiFi.begin(ssid, password);
  
 display.ssd1306_command(SSD1306_DISPLAYON);
 display.setTextSize(1);
 display.setTextColor(WHITE);
 
 int counter = 0; 
 while (WiFi.status() != WL_CONNECTED) //if not connected to wifi
 {
   Serial.print("."); 
   display.clearDisplay();
   display.setCursor(10,0);
   display.print("Connecting");
   display.setCursor(10,9);
   display.print("to wifi: ");
   display.print(counter);
   display.display();
   delay(1500);
   counter++;

   if (counter > 5)
   {
    display.clearDisplay();
    display.setCursor(15,0);
    display.println("No WiFi");
    Serial.println("No WiFi");
    display.display();
    delay(500);
    break;
   }

   if (WiFi.status() == WL_CONNECTED)  //if it connects to wifi
   {
    display.clearDisplay();
    display.setCursor(10,0);
    display.print("Connected!");
    display.display();
    digitalWrite(GREEN_LED, LOW);
    delay(1000);
    digitalWrite(GREEN_LED, HIGH);
    delay(1500);
    display.clearDisplay();

    display.setCursor(0,0);
    IPAddress myIP = WiFi.softAPIP();
    display.println(myIP);
    display.print("RSSI:");
    display.print(WiFi.RSSI());
    delay(1000);  
   }
 }
  
  display.display();
  delay(1000);
   
  Serial.println("LIS3DH test!");
  
  if (! lis.begin(0x19)) 
  {  
    Serial.println("Couldnt start");
    while (1);
  }
  Serial.println("LIS3DH found!");
  
  lis.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G!
  Serial.print("Range = "); Serial.print(2 << lis.getRange());  
  Serial.println("G");
  
  touchAttachInterrupt(T0, gotTouch1, threshold);
  touchAttachInterrupt(T7, gotTouch2, threshold);
  
  delay(2000);
  display.clearDisplay(); 
}

void loop()
{
  lis.read(); 
  sensors_event_t event; 
  lis.getEvent(&event);

  display.ssd1306_command(SSD1306_DISPLAYON);
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0,0);  //over,down
  display.print(hdc1080.readTemperature());
  display.print((char)247); // degree symbol 
  display.print("C  ");
  display.print(hdc1080.readHumidity());
  display.print("%");
  
  display.setCursor(0,9);  //over,down
  display.print(event.acceleration.x);
  display.print(" ");
  display.print(event.acceleration.y);
  display.print(" ");
  display.print(event.acceleration.z);

  while(touch1detected)
  {
    Serial.println("Touch 0 detected");
    //display.clearDisplay();
    display.setCursor(0,0);;
    display.print("Left Touch");
    digitalWrite(RED_LED, LOW);    //On
    delay(50);
    digitalWrite(RED_LED, HIGH);   //Off
    delay(50);
    touch1detected = false;
    
    display.display();
    delay(50);
    display.clearDisplay();
  }
  while(touch2detected)
  {
    Serial.println("Touch 2 detected");
    display.setCursor(0,0);;
    display.print("Right Touch");
    digitalWrite(BLUE_LED, LOW); 
    delay(50);
    digitalWrite(BLUE_LED, HIGH);
    delay(50);
    touch2detected = false;
    
    display.display();
    delay(50);
    display.clearDisplay();
  }

  display.display();
  delay(150);
  display.clearDisplay();
}

void blinkRGB(void)
{
  digitalWrite(RED_LED, LOW);   //led on
  delay(150);
  digitalWrite(RED_LED, HIGH);   //led off
  delay(150);
  digitalWrite(GREEN_LED, LOW); 
  delay(150);
  digitalWrite(GREEN_LED, HIGH);
  delay(150);
  digitalWrite(BLUE_LED, LOW); 
  delay(150);
  digitalWrite(BLUE_LED, HIGH);
  delay(150);
}
