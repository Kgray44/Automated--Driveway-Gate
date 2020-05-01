//blynk
char BLYNK_AUTH_TOKEN[] = "xRH8NnsHrOGi9utKIbLg0pp5Q5gaHtE9"
#define BLYNK_GREEN     "#23C48E"
#define BLYNK_RED       "#D3435C"
#define BLYNK_YELLOW    "#ED9D00"
#define BLYNK_BLUE      "#04C0F8"
#define MY_GATE_LAT ""
#define MY_GATE_LON ""
//WiFi
char WIFI_SSID[] = "Apple Network 85064d";
char WIFI_PASS[] = "12344321";

//GATE
String Mode = "button";      //automatic or button
#define gateNumber 2          //how many gates; 1 or 2
int gateOpenAmount = 10;     //only for automatic mode, seconds
#define gateDirection 1       //1 or 2

//linear actuator
#define inchesPerSec .39      //inches per sec of linear actuator
#define maxActuatorStroke 12  //max stroke of linear actuator
#define percentOfMaxStroke 1  //0 through 1
#define a1 2                  //first linear actuator
#define a2 0                  //first linear actuator
#define b1 4                  //second linear actuator
#define b2 5                  //second linear actuator
