//settings
boolean notifications = true;
boolean houseAlarm = true;

//magnet lock
#define MagnetRelay D7

//car input
#define car D0

//lights
boolean ledStrip = true;
#define red   D1
#define green D8
#define blue  D2

//WiFi
char WIFI_SSID[] = "************";
char WIFI_PASS[] = "*******";
const char passWord[] = "maker";       //password of this device for OTA

//blynk
#define BLYNK_AUTH_TOKEN "********************************"
#define BLYNK_AUTH_TOKEN_2 "********************************"
#define BLYNK_GREEN     "#23C48E"
#define BLYNK_RED       "#D3435C"
#define BLYNK_YELLOW    "#ED9D00"
#define BLYNK_BLUE      "#04C0F8"
#define YOUR_LAT        43.419868
#define YOUR_LONG       -72.467632

//adafruit IO
#define IO_USERNAME "*********"
#define IO_KEY "***_****************"

//GATE
#define gateNumber 2          //how many gates; 1 or 2
#define gateDirection 1       //1 or 2
int gateOpenAmount = 10;      //only for automatic mode, seconds; only for on boot
String Mode = "button";       //automatic or button; only for on boot

//linear actuator
#define inchesPerSec 0.39      //inches per sec of linear actuator
#define maxActuatorStroke 12  //max stroke of linear actuator
#define percentOfMaxStroke 0.8 //0 through 1

#define a1 D3                  //first linear actuator
#define a2 D4                  //first linear actuator
#define b1 D5                  //second linear actuator
#define b2 D6                  //second linear actuator
