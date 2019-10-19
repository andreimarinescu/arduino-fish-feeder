#include "pitches.h"
#include "secrets.h"

#define     VERSION_NUMBER  "0.4.5"

#define     LCD_CHAR_HEART  7
#define     LCD_CHAR_FISH   6

int SERVO_PIN = 9;
int TRIGGER_PIN = 10;
int SET_PIN = 8;
int WIFI_RX = 11;
int WIFI_TX= 12;
int BUZZER_PIN = 13;

int CLOSED_ANGLE = 0;
int OPEN_ANGLE = 180;
int CYCLES_PER_FEED = 3;
int BUZZ_DURATION = 700;
long FEED_INTERVAL = 600000;

int sound1[3] = {
  NOTE_C4, NOTE_C4, NOTE_C4
};

int sound2[8] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  8, 8, 8, 4, 4, 4, 4, 4
};

byte heart[8]= {
  B01010,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000,
  B00000,
  B00000,
};

byte fish[8]= {
  B00000,
  B10100,
  B11110,
  B11111,
  B11110,
  B10100,
  B00000,
  B00000,
};