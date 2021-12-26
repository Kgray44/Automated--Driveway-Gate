# Automated-Driveway-Gate

![IMG_7829](https://user-images.githubusercontent.com/64373011/147396102-93ef6a0b-6274-4e68-8b66-8fe1e50ea647.jpeg)

![IMG_0532](https://user-images.githubusercontent.com/64373011/147396038-53ac4bd5-04f0-4970-9bf6-e07dc68c8bf8.jpeg)

## Info
Automated Driveway Gate using ESP8266, Arduino, and Blynk.

Tutorial here: https://www.hackster.io/k-gray/automated-driveway-gates-ef5c75

Old Outdated Versions:
"Driveway-Gate.ino"
"New_Driveway_Gates.ino"

Old Usable Versions:
"V1"
"V2"
"V3"
"V4"
"V5"
"V6"

Newest/Official Version:
"V7"


## Latest Version's Features
RGB LED strip compatibility (not WS2812)

ZeRGBa Blynk widget for LED strip(s)

Indoor alarm

Adafruit and Blynk connectivity

IFTTT / Adafruit compatibility

Blynk compatibility

Speed / ETA widgets (not fully functioning at the moment)

OTA programming

Distance to gate(s) calculation

Electromagnetic lock compatibility

Blynk gate button changes according to gate status

2 modes; Automatic or button

Distance opening / closing

3 privacy modes for LEDs

Open / close test buttons on Blynk

RSSI widget

All settings in seperate "settings.h" file

waitForGate() function altered; now useable for all linear actuators


## Some More Tips on the Latest Version
- At the beginning of "Automated_Driveway_Gates.ino", you can #define ZeRGBa if you would like to use that widget in Blynk.  If not, just delete that definition and the LED strip will be controlled by the menu option as in previous releases.
- waitForGate() function has been updated to work for any size or speed linear actuator.  The timing for this function does not work as strictly as hoped though, because of the slight delay from Blynk.  So if you would rather, you can uncomment the commented section in this function, and comment the uncommented section.  The variable "tim" must be changed as it controls for how long the gate(s) open / close.  The variable "tim" is an arbitrary value, not a measurement in any way.


## Adding to Arduino IDE
1. Go to [releases](https://github.com/Kgray44/Automated--Driveway-Gate/releases) and download the latest version.

2. Unzip and open the downloaded file.

3. Open the correct version folder.

4. Open the "Arduino" home folder in a new finder window.

5. Click and drag the file inside the version file into the "Arduino" folder.

6. Reboot the Arduino IDE.

7. Installed!
