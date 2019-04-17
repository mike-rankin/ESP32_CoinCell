# ESP32_CoinCell

Video of it in action is at: https://youtu.be/IdSCKr7fLYc

Rev1 soon to be Rev4: https://www.tindie.com/products/miker/esp32-coincell/

This is an ESP32 Pico D4 project with an accelerometer and 0.69" oled display powered by a rechargeable LIR2450 coin cell. I was not made for any specific purpose and was really more of a design challenge to try and made it as small as possible and my first attempt at running something at low power. It can be powered off of a battery or USB cable. If a battery is inserted and USB cable is plugged in it will charge the battery and power the board. Current consumption is around 0.30uA when sleeping but up to 85mA running. Runtime testing is still in the works but battery life is only 5 minutes when the display is on and constantly connected to wifi. Connecting to wifi and turning on the display every 10 minutes appears to last 10 hours or so. Revision 1 started off at 200uA in sleep mode. Revisions after that used a switching power supply and setting registers in the accelerometer but they all ended in fail. This revision 4 with a much better LDO and powering the accelerometer off of the ESP32 has finally got the sleep current to 30uA. The design files and parts list is provided if you would like to assemble your own. If you can come up with a specific use for this hardware or have any questions then just let me know at @mikerankin or by email at 0miker0@gmail.com

Main components used in this desgin are:

-ESP32 Pico D4 (wifi/bluetooth processor)

-LIR2450 (3.7V battery) or optional external battery using the microJST connector

-HT7833 (500mA LDO power supply)

-SL4054ST25P (LiPo battery charger)

-LIS3DHTR (accelerometer) now powered off an ESP32 GPIO pin

-CP2102N (USB interface chip)

-ER-OLED0.69-1W (96x16 oled dsiplay)

The full parts list and schematic is in the PCB directory

The bare PCB was provided by JLCPCB and most of the parts by LCSC.

![Rev4](https://user-images.githubusercontent.com/4991664/56325424-85181b80-6148-11e9-81cc-3ab20f2d0517.jpg)
