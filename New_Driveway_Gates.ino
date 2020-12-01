#define BLYNK_PRINT Serial

#include "settings.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <math.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

char auth[] = BLYNK_AUTH_TOKEN;

int beginning=6;
int ending=18;
int timer = 0;
int timer1 = 0;
int timer2 = 0;
int button;
int emergency;
int setting;
int delayAMOUNT;
int gpsSelect;
int distanceSelect;
int comingHome;
int openTest;
int closeTest;
float GPSLat;
float GPSLong;

float dist_calc=0;
float dist_calc2=0;
float diflat=0;
float diflon=0;

boolean away = false;

Adafruit_BME280 bme;
WidgetRTC rtc;
WidgetBridge bridge1(V77);

int counter=0;
int p=0;
int pre=0;

BLYNK_CONNECTED() {
  Blynk.syncAll();
  bridge1.setAuthToken("Td-96Moiv04el-IIM-TruwgdZejlegml");
  Serial.println("Syncing...");
  delay(2000);
}
BLYNK_WRITE(V0){
  button = param.asInt();
}
BLYNK_WRITE(V17){
  emergency = param.asInt();
}
BLYNK_WRITE(V16){
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

BLYNK_WRITE(V15){
  delayAMOUNT = param.asInt();
}
BLYNK_WRITE(V14){
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
BLYNK_WRITE(V20){
  pressureReadings = param.asInt();
}
BLYNK_WRITE(V18){
  beginning = param.asInt();
}
BLYNK_WRITE(V19){
  ending = param.asInt();
}
void setup() {
  Serial.begin(9600);
  unsigned status;
  status = bme.begin(0x76);
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
    Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
  }
  ArduinoOTA.setHostname("DrivewayESP8266");
  ArduinoOTA.setPassword("!KGray7777!");
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
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
  pinMode(emergencyButton, INPUT_PULLUP);
  Blynk.begin(BLYNK_AUTH_TOKEN,WIFI_SSID,WIFI_PASS);
  rtc.begin();
  led.on();
  led.setColor(BLYNK_RED);
  manualoff();
  digitalWrite(0, HIGH);
  digitalWrite(2, HIGH);
}

void loop() {
  ArduinoOTA.handle();
  if (hour() == 8){
    if (minute() == 1){
      if (second() == 1){
        Blynk.email("kkidsgray@icloud.com", "Personal Weather Update", "Welcome to the daily weather update. The temperature today is " + String((bme.readTemperature()*1.8)+32) + ". The humidity is " + String(bme.readHumidity()) + ". Barometric pressure is " + String(bme.readPressure() / 100.0) + " mBar and " + String((bme.readPressure()/100)/32.75) + " inches.");
        Blynk.email("kristinlgray@yahoo.com", "Personal Weather Update", "Welcome to the daily weather update. The temperature today is " + String((bme.readTemperature()*1.8)+32) + ". The humidity is " + String(bme.readHumidity()) + ". Barometric pressure is " + String(bme.readPressure() / 100.0) + " mBar and " + String((bme.readPressure()/100)/32.75) + " inches.");
      }
    }
  }
  counter++;
  Serial.println(counter);
  if (counter == pressureReadings){
    if (pre < p/counter){
      Blynk.virtualWrite(V27, 0);
      Serial.println("pre < p/counter");
    }
    else if (pre > p/counter){
      Blynk.virtualWrite(V27, 1);
      Serial.println("pre > p/counter");
    }
    else if (pre == p/counter){
      Blynk.virtualWrite(V27, 0.5);
      Serial.println("pre == p/counter");
    }
    pre = p/counter;
    
    counter=0;
    p=0;
  }
  p+=(bme.readPressure()/100.0);
  Serial.println(p);
  
  Blynk.virtualWrite(V11, WiFi.RSSI());
  Blynk.virtualWrite(V4, (bme.readTemperature()*1.8)+32);
  Blynk.virtualWrite(V5, bme.readHumidity());
  Blynk.virtualWrite(V1, bme.readPressure() / 100.0);
  Blynk.virtualWrite(V2, (bme.readPressure()/100)/32.75);
  if (emergencyButton == LOW){
    Serial.println("Emergency Button!");
    Blynk.notify("Emergency Button on Gates was just Pressed!!!");
    gates();
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
    delay(500);
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
    //if (comingHome == 1){
    if (calcDist(GPSLat, GPSLong, YOUR_LAT, YOUR_LONG) < distanceSelect){
      away=false;
    }
    else if (calcDist(GPSLat, GPSLong, YOUR_LAT, YOUR_LONG) > distanceSelect){
      away=true;
    }
    if (away==true){
      Serial.println("Away = true");
      //Serial.println(calcDist(GPSLat, GPSLong, YOUR_LAT, YOUR_LONG));
      if (calcDist(GPSLat, GPSLong, YOUR_LAT, YOUR_LONG) < distanceSelect && calcDist(GPSLat, GPSLong, YOUR_LAT, YOUR_LONG) > (distanceSelect - 100)){
        Serial.println("GPS triggered!");
        Mode = "button";
        timer=0;
        gates();
      }
    }
  }
  Blynk.run();
}

void gates(){
  Serial.println("Got Mail!");
  if (Mode == "automatic"){
    Serial.println("Mode = Automatic");
    Blynk.notify("Driveway Gates were opened!");
    Serial.println("Opening gate...");
    led.setColor(BLYNK_BLUE);
    offMagnet();
    openGate();
    waitForGate();
    allStop();
    led.setColor(BLYNK_GREEN);
    Serial.println("Open.");
    Serial.println("Waiting...");
    delay(gateOpenAmount*1000);
    Blynk.notify("Driveway Gates were closed!");
    Serial.println("Closing gate...");
    led.setColor(BLYNK_BLUE);
    closeGate();
    waitForGate();
    allStop();
    onMagnet();
    led.setColor(BLYNK_RED);
    Serial.println("Closed.");
  }
  else if(Mode == "button"){
    Serial.println("Mode = Button");
    if (timer == 0){
      Blynk.notify("Driveway Gates were opened!");
      Serial.println("Openning gate...");
      led.setColor(BLYNK_BLUE);
      offMagnet();
      openGate();
      waitForGate();
      allStop();
      led.setColor(BLYNK_GREEN);
      Serial.println("Open.");
      timer = 1;
    }
    else if(timer == 1){
      Blynk.notify("Driveway Gates were closed!");
      Serial.println("Closing gate...");
      led.setColor(BLYNK_BLUE);
      closeGate();
      waitForGate();
      allStop();
      onMagnet();
      led.setColor(BLYNK_RED);
      Serial.println("Closed.");
      timer = 0;
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
  if (hour() >= beginning && hour() <= ending){
    for(int i;i<15;i++){
      bridge1.virtualWrite(V50, 1);
      delay(1000);
      bridge1.virtualWrite(V51, 0);
      delay(500);
    }
  }
}

void closeGate(){
  //if (gateDirection == 1){
  //  if (gateNumber == 1){
      //digitalWrite(a1, LOW);
      //digitalWrite(a2, HIGH);
  //  }
  //  else if (gateNumber == 2){
      digitalWrite(a1, LOW);
      digitalWrite(a2, HIGH);
      digitalWrite(b1, LOW);
      digitalWrite(b2, HIGH);
   /* }
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
  */
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
  digitalWrite(en1, LOW);
  digitalWrite(en2, LOW);
}

void manualoff() {
  digitalWrite(en1, HIGH);
  digitalWrite(en2, HIGH);
}

void waitForGate() {
  int tim = 5500;
  Begin:
  timer1++;
  while(timer1 < tim){
    Blynk.run();
    delay(2);
    Serial.println(timer1);
    goto Begin;
  }
  timer1 = 0;
  //delay((inchesPerSec*(maxActuatorStroke/percentOfMaxStroke))*1400);
}

void onMagnet(){
  digitalWrite(MagnetRelay, HIGH);
}

void offMagnet(){
  digitalWrite(MagnetRelay, LOW);
}


float calcDist(float CurrentLatitude, float CurrentLongitude, float SavedLatitude, float SavedLongitude)
{
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
    Blynk.virtualWrite(V7, Distance);
    return(Distance);
}
