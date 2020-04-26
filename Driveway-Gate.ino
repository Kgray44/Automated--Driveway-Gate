#define BLYNK_PRINT Serial

#include "settings.h"
#include "AdafruitIO_WiFi.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

boolean car = false;
int timer = 0;
int button;

BLYNK_WRITE(V0){
  button = param.asInt();
}

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
AdafruitIO_Feed *gate_feed = io.feed(GATE_FEED);
AdafruitIO_Feed *car_feed = io.feed(CAR_FEED);

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
  if (IOT_CHOICE == "blynk"){
    Blynk.begin(WIFI_SSID,WIFI_PASS,BLYNK_AUTH_TOKEN);
  }
  else if (IOT_CHOICE == "adafruit"){
    io.connect();
    gate_feed->onMessage(handleMessage);
    while(io.status() < AIO_CONNECTED) {
      Serial.print(".");
      delay(500);
    }
    Serial.println();
    Serial.println(io.statusText());
    gate_feed->get();
  }
  else if (IOT_CHOICE == "both"){
    Blynk.config(auth);
    io.connect();
    gate_feed->onMessage(handleMessage);
    while(io.status() < AIO_CONNECTED) {
      Serial.print(".");
      delay(500);
    }
    Serial.println();
    Serial.println(io.statusText());
    gate_feed->get();
    Blynk.disconnect();
  }
}

void loop() {
  if ((IOT_CHOICE == "adafruit")||(IOT_CHOICE == "both")){
    io.run();
  }
  if (IOT_CHOICE == "blynk"){
    Blynk.run();
    if (button == 1){
      gates();
    }
  }
}

void handleMessage(AdafruitIO_Data *data) {
  Serial.println("Got Mail!");
  if (Mode == "automatic"){
    openGate();
    waitForGate();
    allStop();
    delay(gateOpenAmount*1000);
    closeGate();
    waitForGate();
    allStop();
  }
  else if(Mode == "button"){
    if (timer == 0){
      openGate();
      waitForGate();
      allStop();
      timer = 1;
    }
    else if(timer == 1){
      closeGate();
      waitForGate();
      allStop();
      timer = 0;
    }
  }
}

void gates(){
  Serial.println("Got Mail!");
  if (Mode == "automatic"){
    openGate();
    waitForGate();
    allStop();
    delay(gateOpenAmount*1000);
    closeGate();
    waitForGate();
    allStop();
  }
  else if(Mode == "button"){
    if (timer == 0){
      openGate();
      waitForGate();
      allStop();
      timer = 1;
    }
    else if(timer == 1){
      closeGate();
      waitForGate();
      allStop();
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

void waitForGate() {
  delay(inchesPerSec*(maxActuatorStroke/percentOfMaxStroke));
}
