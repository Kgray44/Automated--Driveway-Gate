//pins
#define MagnetRelay D7

//lights
boolean ledStrip = true;
#define red   D1
#define green D8
#define blue  D2

//blynk and adafruit
#define BLYNK_AUTH_TOKEN "xRH8NnsHrOGi9utKIbLg0pp5Q5gaHtE9"
#define BLYNK_AUTH_TOKEN_2 "nGIhBbKBYHv9wGH2umEIvGOkwDwJ24ay"
#define BLYNK_GREEN     "#23C48E"
#define BLYNK_RED       "#D3435C"
#define BLYNK_YELLOW    "#ED9D00"
#define BLYNK_BLUE      "#04C0F8"
#define YOUR_LAT        43.419868
#define YOUR_LONG       -72.467632
#define IO_USERNAME     "KGray7777"
#define IO_KEY          "aio_OPzJ16ceXnsDbfToy90CLhmnv8v1"
boolean notifications = true;
boolean houseAlarm = true;
//WiFi
char WIFI_SSID[] = "Apple Network 85064d";
char WIFI_PASS[] = "12344321";
const char passWord[] = "maker";       //password of this device for OTA

//GATE
String Mode = "button";       //automatic or button
#define gateNumber 2          //how many gates; 1 or 2
int gateOpenAmount = 10;      //only for automatic mode, seconds
#define gateDirection 1       //1 or 2

//linear actuator
#define inchesPerSec .39      //inches per sec of linear actuator
#define maxActuatorStroke 12  //max stroke of linear actuator
#define percentOfMaxStroke 0.21 //.2 //0 through 1
#define a1 D3                  //first linear actuator
#define a2 D4                  //first linear actuator
#define b1 D5                  //second linear actuator
#define b2 D6                  //second linear actuator

#include "AdafruitIO_WiFi.h"
/*
#if defined(USE_AIRLIFT) || defined(ADAFRUIT_METRO_M4_AIRLIFT_LITE) ||         \
    defined(ADAFRUIT_PYPORTAL)
// Configure the pins used for the ESP32 connection
#if !defined(SPIWIFI_SS) // if the wifi definition isnt in the board variant
// Don't change the names of these #define's! they match the variant ones
#define SPIWIFI SPI
#define SPIWIFI_SS 10 // Chip select pin
#define NINA_ACK 9    // a.k.a BUSY or READY pin
#define NINA_RESETN 6 // Reset pin
#define NINA_GPIO0 -1 // Not connected
#endif
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS, SPIWIFI_SS,
                   NINA_ACK, NINA_RESETN, NINA_GPIO0, &SPIWIFI);
#else
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
#endif*/
