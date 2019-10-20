#include "pitches.h"
#include "secrets.h"

#define     VERSION_NUMBER  "0.4.6"

#define     LCD_CHAR_HEART  7
// #define     LCD_CHAR_FISH   6

int SET_PIN         = 8;
int SERVO_PIN       = 9;
int TRIGGER_PIN     = 10;
int WIFI_RX         = 11;
int WIFI_TX         = 12;
int BUZZER_PIN      = 13;

int BUZZ_DURATION   = 700;
int CLOSED_ANGLE    = 0;
int CYCLES_PER_FEED = 3;
int OPEN_ANGLE      = 180;
long FEED_INTERVAL  = 3600000;