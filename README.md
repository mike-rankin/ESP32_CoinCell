# ESP32_CoinCell

Video of it in action is at: https://youtu.be/QvrdFFZR3Nw

This is an ESP32 Pico D4 project with an accelerometer and 0.69" oled display powered by a rechargeable LIR2450 coin cell. I was not made for any specific purpose and was a design challenge to try and made it as small as possible. It can be powered off of a battery or USB cable. If a battery is inserted and USB cable is plugged in it will charge the battery and power the board. Current consumption is around 0.45mA when sleeping but up to 85mA running. Runtime testing is still in the works but battery life is only 5 minutes when the display is on and constantly connected to wifi. Connecting to wifi and turning on the display every 10 minutes appears to last 10 hours or so. The design files and parts list is provided if you would like to assemble your own. If you can come up with a specific use for this hardware or have any questions then just let me know at @mikerankin or by email at 0miker0@gmail.com

Main components used in this desgin are:

-ESP32 Pico D4 (wifi/bluetooth processor)

-LIR2450 (3.7V battery)

-SE5218ALG-LF (500mA LDO power supply)

-SL4054ST25P (LiPo battery charger)

-LIS3DHTR (accelerometer)

-CP2102N (USB interface chip)

-ER-OLED0.69-1W (96x16 oled dsiplay)

The full parts list and schematic is in the PCB directory

The bare PCB was provided by JLCPCB and most of the parts by LCSC.

![top_side](https://user-images.githubusercontent.com/4991664/52292089-4c8ae300-294a-11e9-8096-ea8b10b4526b.jpg)
