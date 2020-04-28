//IOT
#define IOT_CHOICE "blynk"    //blynk or adafruit

//adafruit
#define IO_USERNAME "your_username"
#define IO_KEY "your_key"
#define GATE_FEED "gmail"     //incoming

//blynk
#define BLYNK_AUTH_TOKEN "your_auth"
#define MY_MESSAGE "There is a car in your driveway!" //Blynk notification

//WiFi
char WIFI_SSID[] = "your_ssid";
char WIFI_PASS[] = "your_pass";

//GATE
#define Mode "automatic"      //automatic or button
#define gateNumber 2          //how many gates; 1 or 2
#define gateOpenAmount 10     //only for automatic mode, seconds
#define gateDirection 1       //1 or 2

//linear actuator
#define inchesPerSec .39      //inches per sec of linear actuator
#define maxActuatorStroke 16  //max stroke of linear actuator
#define percentOfMaxStroke 1  //0 through 1
#define a1 2                  //first linear actuator
#define a2 0                  //first linear actuator
#define b1 4                  //second linear actuator
#define b2 5                  //second linear actuator
