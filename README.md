# recon-drone

This repository contains a custom fan made code for a [Tom Clancy's Rainbow Six®
Siege — Ubisoft®](https://www.ubisoft.com/en-us/game/rainbow-six/siege) recon drone.
This repository is completely and only fan made, but it was intended to be as realistic based on the same type of recon drone from the game as possible.
It uses a ESP32-CAM module. It is controlled over WiFi as Access Point.

This entire project is based on code from [HemRobotics](https://github.com/hemrobotics/recon-drone), shoutout to him!
This project is mostly a copy of his work, with custom changes based on my own preferences and later improvements.

Please watch and share his work on youtube.
Watch the [HemRobotics recon drone video on YouTube](https://youtu.be/NfWd9Kn30i0) for
complete build guide.

## Schematic diagram
The schematic diagram will be posted later as it is still on breadboard and will need some tuning.


## How to use the code?
- Follow [HemRobotics recon drone video on YouTube](https://youtu.be/NfWd9Kn30i0) and the schematic diagram to make the connections. 
- Open `R6ReconDrone.ino` in Arduino IDE.
- Select correct camera model by commenting / un-commenting correct
  `#define` statement.
- Add SSID and password of your WiFi access point or leave it as it is. Current default settings: `SSID: R6_drone_01` ; `Password: Rainbow6` ; `IP: 192.168.6.6`. DON'T use the same SSID as your home router.
- You can either flash your ESP using [Arduino
  UNO](https://youtu.be/q-KIpFIbRMk) or [FTDI USB to TTL serial
  converter](https://youtu.be/tzmcXZ-irIc).
- Once ESP32 CAM is connected to WiFi, you can get it's IP address in
  the serial monitor of Arduino IDE.

## Licensing & credits
recon-drone is licensed under Apache License Version 2.0. See
[LICENSE](./LICENSE) for the full license text.

This code is based on
[CameraWebServer](https://github.com/espressif/arduino-esp32/tree/master/libraries/ESP32/examples/Camera/CameraWebServer)
by Espressif Systems and
[esp32cam-car](https://github.com/heets-silver/esp32cam-car) by
heets-silver. Both are licensed under Apache License Version 2.0.
