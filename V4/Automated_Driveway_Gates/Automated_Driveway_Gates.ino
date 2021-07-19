/*************************************************************
File Name: Driveway_Gates.ino
Processor/Platform: ESP8266 (D1-Mini tested)
Development Environment: Arduino 1.8.15

Download latest code here:
https://github.com/Kgray44/Automated--Driveway-Gate

Driveway Gates code meant to be used alongside the tutorial found here:
https://www.hackster.io/k-gray/new-automated-driveway-gates-ef5c75


Copyright 2021 K Gray

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Liscense found here:
https://opensource.org/licenses/MIT
 *************************************************************/


#define BLYNK_PRINT Serial

#include "settings.h"
#include <math.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

//#include <AdafruitIO_WiFi.h>

//AdafruitIO_WiFi io("KGray7777", "aio_OPzJ16ceXnsDbfToy90CLhmnv8v1", "Apple Network 85064d", "12344321");
//AdafruitIO_Feed *car = io.feed("car");

char auth[] = BLYNK_AUTH_TOKEN;
char auth2[] = BLYNK_AUTH_TOKEN_2;

int flag1 = 0;
int flag2 = 0;
int button;
int emergency;
int setting;
int delayAMOUNT;
int gpsSelect;
int distanceSelect;
int comingHome;
int openTest;
int closeTest;
int privacyMode;
int lightsIn;

float GPSLat;
float GPSLong;

float dist_calc=0;
float dist_calc2=0;
float diflat=0;
float diflon=0;

WidgetBridge alarm(V14);

BLYNK_CONNECTED() {
  alarm.setAuthToken(auth2);
  Blynk.syncAll();
  Serial.println("Syncing...");
  delay(1000);
}
BLYNK_WRITE(V0){
  button = param.asInt();
}
BLYNK_WRITE(V1){
  emergency = param.asInt();
}
BLYNK_WRITE(V2){
  setting = param.asInt();
  if (setting == 1) {
    Mode = "automatic";
  }
  else if (setting == 0) {
    Mode = "button";
  }
}
BLYNK_WRITE(V3){
  gateOpenAmount = param.asInt();
}

BLYNK_WRITE(V4){
  delayAMOUNT = param.asInt();
}
BLYNK_WRITE(V5){
  GpsParam gps(param);
  GPSLat = gps.getLat();
  GPSLong = gps.getLon();
}
WidgetLED led(V6);
BLYNK_WRITE(V8){
  gpsSelect = param.asInt();
}
BLYNK_WRITE(V9){
  distanceSelect = param.asInt();
}
BLYNK_WRITE(V10){
  comingHome = param.asInt();
}
BLYNK_WRITE(V12){
  openTest = param.asInt();
}
BLYNK_WRITE(V13){
  closeTest = param.asInt();
}
BLYNK_WRITE(V15){
  privacyMode = param.asInt();
}
BLYNK_WRITE(V16){
  lightsIn = param.asInt();
}

void setup() {
  Serial.begin(9600);
  if (ledStrip == true){
    pinMode(red, OUTPUT);
    pinMode(green, OUTPUT);
    pinMode(blue, OUTPUT);
  }
  if (gateNumber == 1){
    pinMode(a1, OUTPUT);
    pinMode(a2, OUTPUT);
  }
  else if (gateNumber == 2){
    pinMode(a1, OUTPUT);
    pinMode(a2, OUTPUT);
    pinMode(b1, OUTPUT);
    pinMode(b2, OUTPUT);
  }
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID,WIFI_PASS);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  OTAStart();
  Blynk.config(BLYNK_AUTH_TOKEN);
  //io.connect();//IO_USERNAME, IO_KEY
  //car->onMessage(handleMessage);
  //while(io.mqttStatus() < AIO_CONNECTED) {
  //  Serial.print(".");
  //  delay(500);
  //}
  //car->get();

  led.on();
  led.setColor(BLYNK_RED);
  alarm.virtualWrite(V1, LOW);
}

void loop() {
  ArduinoOTA.handle();
  Blynk.virtualWrite(V11, WiFi.RSSI());
  if (flag1 == 0){
    ledtrip();
  }
  if (privacyMode == 2 || privacyMode == 3){
    ledstrip(0,0,0);
  }
  if (openTest == 1){
    Serial.println("open test");
    openGate();
    delay(500);
  }
  else {
    allStop();
  }
  if (closeTest == 1){
    Serial.println("close test");
    closeGate();
  }
  else {
    allStop();
  }
  if (button == 1){
    Serial.println("Button pressed.");
    gates();
  }
  if (emergency == 1){
    Serial.println("M");
    led.setColor(BLYNK_YELLOW);
    manual();
    delay(delayAMOUNT*1000);
    led.setColor(BLYNK_RED);
  }
  if (gpsSelect != 1){
    Blynk.virtualWrite(V7,calcDist(GPSLat, GPSLong, YOUR_LAT, YOUR_LONG));
    if (comingHome == 1){
      Serial.println(calcDist(GPSLat, GPSLong, YOUR_LAT, YOUR_LONG));
      if (calcDist(GPSLat, GPSLong, YOUR_LAT, YOUR_LONG) < distanceSelect){
        Serial.println("GPS triggered!");
        opengates();
      }
    }
  }
  Blynk.run();
//  io.run();
}

void gates(){
  Serial.println("Got Mail!");
  if (Mode == "automatic"){
    Serial.println("Mode = Automatic");
    if (notifications == true){
      Blynk.notify("Driveway Gates were opened!");
    }
    Serial.println("Opening gate...");
    led.setColor(BLYNK_BLUE);
    if (privacyMode == 1 || privacyMode == 2){
      ledstrip(0,0,255);//blue
    }
    //if (houseAlarm == true){
      alarm.virtualWrite(V1, HIGH);
    //}
    magnet(LOW);
    openGate();
    waitForGate();
    allStop();
    led.setColor(BLYNK_GREEN);
    if (privacyMode == 1 || privacyMode == 2){
      ledstrip(0,255,0);//green
    }
    Serial.println("Open.");
    Serial.println("Waiting...");
    delay(gateOpenAmount*1000);
    if (notifications == true){
      Blynk.notify("Driveway Gates were closed!");
    }
    Serial.println("Closing gate...");
    led.setColor(BLYNK_BLUE);
    if (privacyMode == 2){
      ledstrip(0,0,255);//blue
    }
    closeGate();
    waitForGate();
    allStop();
    magnet(HIGH);
    led.setColor(BLYNK_RED);
    //turn leds on red for 4 seconds after shutting
    ledstrip(255,0,0);//red
    delay(4000);
    ledstrip(0,0,0);//off
    Serial.println("Closed.");
  }
  else if(Mode == "button"){
    Serial.println("Mode = Button");
    if (flag1 == 0){
      if (notifications == true){
        Blynk.notify("Driveway Gates were opened!");
      }
      Serial.println("Opening gate...");
      led.setColor(BLYNK_BLUE);
      if (privacyMode == 1 || privacyMode == 2){
        ledstrip(0,0,255);//blue
      }
      //if (houseAlarm == true){
        alarm.virtualWrite(V1, HIGH);
      //}
      magnet(LOW);
      openGate();
      waitForGate();
      allStop();
      led.setColor(BLYNK_GREEN);
       if (privacyMode == 1 || privacyMode == 2){
        ledstrip(0,255,0);//green
      }
      Serial.println("Open.");
      flag1 = 1;
    }
    else if(flag1 == 1){
      if (notifications == true){
        Blynk.notify("Driveway Gates were closed!");
      }
      Serial.println("Closing gate...");
      led.setColor(BLYNK_BLUE);
      if (privacyMode == 1 || privacyMode == 2){
        ledstrip(0,0,255);//blue
      }
      closeGate();
      waitForGate();
      allStop();
      magnet(HIGH);
      led.setColor(BLYNK_RED);
      if (privacyMode == 1 || privacyMode == 2){
        ledstrip(255,0,0);//red
      }
      Serial.println("Closed.");
      flag1 = 0;
    }
  }
}

void openGate(){
  if (gateDirection == 1){
    if (gateNumber == 1){
      digitalWrite(a1, HIGH);
      digitalWrite(a2, LOW);
    }
    else if (gateNumber == 2){
      digitalWrite(a1, HIGH);
      digitalWrite(a2, LOW);
      digitalWrite(b1, HIGH);
      digitalWrite(b2, LOW);
    }
  }
  else if (gateDirection == 2){
    if (gateNumber == 1){
      digitalWrite(a1, LOW);
      digitalWrite(a2, HIGH);
    }
    else if (gateNumber == 2){
      digitalWrite(a1, LOW);
      digitalWrite(a2, HIGH);
      digitalWrite(b1, LOW);
      digitalWrite(b2, HIGH);
    }
  }
}

void closeGate(){
  if (gateDirection == 1){
    if (gateNumber == 1){
      digitalWrite(a1, LOW);
      digitalWrite(a2, HIGH);
    }
    else if (gateNumber == 2){
      digitalWrite(a1, LOW);
      digitalWrite(a2, HIGH);
      digitalWrite(b1, LOW);
      digitalWrite(b2, HIGH);
    }
  }
  else if (gateDirection == 2){
    if (gateNumber == 1){
      digitalWrite(a1, HIGH);
      digitalWrite(a2, LOW);
    }
    else if (gateNumber == 2){
      digitalWrite(a1, HIGH);
      digitalWrite(a2, LOW);
      digitalWrite(b1, HIGH);
      digitalWrite(b2, LOW);
    }
  }
}

void allStop() {
  if (gateNumber == 1){
    digitalWrite(a1, LOW);
    digitalWrite(a2, LOW);
  }
  else if (gateNumber == 2){
    digitalWrite(a1, LOW);
    digitalWrite(a2, LOW);
    digitalWrite(b1, LOW);
    digitalWrite(b2, LOW);
  }
}

void manual() {
  if (gateNumber == 1){
    digitalWrite(a1, HIGH);
    digitalWrite(a2, HIGH);
  }
  else if (gateNumber == 2){
    digitalWrite(a1, HIGH);
    digitalWrite(a2, HIGH);
    digitalWrite(b1, HIGH);
    digitalWrite(b2, HIGH);
  }
}

void waitForGate() {
  int tim = 5500;
  Begin:
  flag2++;
  while(flag2 < tim){
    Blynk.run();
    delay(2);
    Serial.println(flag2);
    goto Begin;
  }
  flag2 = 0;
  //delay((inchesPerSec*(maxActuatorStroke/percentOfMaxStroke))*1400);
}

void opengates(){
  Serial.println("Mode = Button");
  Serial.println("Openning gate...");
  led.setColor(BLYNK_BLUE);
  openGate();
  waitForGate();
  allStop();
  led.setColor(BLYNK_GREEN);
  Serial.println("Open.");
  flag1 = 1;
}

void magnet(boolean stat){
  digitalWrite(MagnetRelay, stat);
}

float calcDist(float CurrentLatitude, float CurrentLongitude, float SavedLatitude, float SavedLongitude){
// HaverSine version
  const float Deg2Rad = 0.01745329252;               // (PI/180)  0.017453293, 0.0174532925
  //const double EarthRadius = 6372.795;              //6372.7976 In Kilo meters, will scale to other values
  const float EarthRadius = 20908120.1;              // In feet  20908128.6
  float DeltaLatitude, DeltaLongitude, a, Distance;

  // degrees to radians
  CurrentLatitude = (CurrentLatitude + 180) * Deg2Rad;     // Remove negative offset (0-360), convert to RADS
  CurrentLongitude = (CurrentLongitude + 180) * Deg2Rad;
  SavedLatitude = (SavedLatitude + 180) * Deg2Rad;
  SavedLongitude = (SavedLongitude + 180) * Deg2Rad;

  DeltaLatitude = SavedLatitude - CurrentLatitude;
  DeltaLongitude = SavedLongitude - CurrentLongitude;

  a =(sin(DeltaLatitude/2) * sin(DeltaLatitude/2)) + cos(CurrentLatitude) * cos(SavedLatitude) * (sin(DeltaLongitude/2) * sin(DeltaLongitude/2));
  Distance = EarthRadius * (2 * atan2(sqrt(a),sqrt(1-a)));
  return(Distance);
}

void OTAStart(){
  // Port defaults to 8266
   ArduinoOTA.setPort(8266);
  // Hostname defaults to esp8266-[ChipID]
   ArduinoOTA.setHostname("DrivewayESP");
  // No authentication by default
   ArduinoOTA.setPassword(passWord);//(const char 

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void ledtrip(){
  if (privacyMode == 1){ //normal
    if (lightsIn == 1){
      ledstrip(255,0,0); //red
    }
    if (lightsIn == 2){
      ledstrip(255,145,0); //orange
    }
    if (lightsIn == 3){
      ledstrip(255,255,0); //yellow
    }
    if (lightsIn == 4){
      ledstrip(0,255,0); //green
    }
    if (lightsIn == 5){
      ledstrip(0,0,255); //Blue
    }
    if (lightsIn == 6){
      ledstrip(145,0,255); //Purple
    }
    if (lightsIn == 7){
      ledstrip(255,255,255); //white
    }
  }
}

void ledstrip(int r, int g, int b){
  analogWrite(red, r);
  analogWrite(green, g);
  analogWrite(blue, b);
}

/*void handleMessage(AdafruitIO_Data *data) {
  Serial.print("received <- ");
  Serial.println(data->value());
  int dat = data->toInt();
  if (dat == 1){
    opengates();
  }
}*/
