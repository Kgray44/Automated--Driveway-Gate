#define BLYNK_PRINT Serial

#include "settings.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <math.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

char auth[] = BLYNK_AUTH_TOKEN;

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

BLYNK_CONNECTED() {
  Blynk.syncAll();
  Serial.println("Syncing...");
  delay(2000);
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

void setup() {
  Serial.begin(9600);
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
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID,WIFI_PASS);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  OTAStart();
  Blynk.config(BLYNK_AUTH_TOKEN);
  led.on();
  led.setColor(BLYNK_RED);
}

void loop() {
  ArduinoOTA.handle();
  Blynk.virtualWrite(V11, WiFi.RSSI());
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

void opengates(){
  Serial.println("Mode = Button");
  Serial.println("Openning gate...");
  led.setColor(BLYNK_BLUE);
  openGate();
  waitForGate();
  allStop();
  led.setColor(BLYNK_GREEN);
  Serial.println("Open.");
  timer = 1;
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

void OTAStart(){
  // Port defaults to 8266
   ArduinoOTA.setPort(8266);
  // Hostname defaults to esp8266-[ChipID]
   ArduinoOTA.setHostname("DrivewayESP");
  // No authentication by default
   ArduinoOTA.setPassword((const char *)"driveway");

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
