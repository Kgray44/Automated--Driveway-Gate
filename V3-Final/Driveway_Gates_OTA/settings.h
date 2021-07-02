#define emergencyButton D8
#define MagnetRelay D7
//blynk
#define BLYNK_AUTH_TOKEN " xRH8NnsHrOGi9utKIbLg0pp5Q5gaHtE9"
#define BLYNK_GREEN     "#23C48E"
#define BLYNK_RED       "#D3435C"
#define BLYNK_YELLOW    "#ED9D00"
#define BLYNK_BLUE      "#04C0F8"
#define YOUR_LAT        43.419868
#define YOUR_LONG       -72.467632
#define IO_USERNAME     "KGray7777"
#define IO_KEY          "aio_VUZo11AfxVJP7h3l1oEZPxNEKSxO"

//WiFi
char WIFI_SSID[] = "Apple Network 85064d";
char WIFI_PASS[] = "12344321";

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
