#include "pitches.h"
#include <Servo.h>
#include <LiquidCrystal.h> // LCD screen driver 

LiquidCrystal lcd(2, 3, 4, 5, 6, 7); // Parameters: (rs, enable, d4, d5, d6, d7) 
Servo servo;

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

boolean auto_enabled = true;
unsigned long last_feed = 0;
unsigned long last_status_print = 0;
int num_feeds = 0;

String version_number = "0.2.1";

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

void setup() {
  servo.attach(SERVO_PIN);
  pinMode(TRIGGER_PIN, INPUT);
  pinMode(SET_PIN, INPUT);

  lcd.begin(16,2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display } 
  lcd.createChar(7, heart);
  lcd.createChar(6, fish);

  print_boot();
  init_servo();

  delay(1000);
}

void play_melody(int melody[], int length) {
    // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < length; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(13, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(13);
  }
}

void buzz(int freq) {
  tone(BUZZER_PIN, freq);
  delay(BUZZ_DURATION);
  noTone(BUZZER_PIN);
}

void print_boot() {
  lcd.setCursor(1,0);
  lcd.write(7);
  lcd.print("Fish  Feeder");
  lcd.write(7);
  lcd.setCursor(8,1);
  lcd.print("v. "+version_number); 
}

void print_status() {
  if(last_status_print + 1000 > millis()) {
    return;
  }
  int uptime_hours = millis() / 1000 / 60 / 60;
  String line_1 = "Auto:";
  if(auto_enabled) {
    line_1.concat("ON  Up:");
  } else {
    line_1.concat("OFF  Up:");
  }
  line_1.concat(uptime_hours);
  line_1.concat("h");
  String line_2 = "Feeds: ";
  line_2.concat(num_feeds);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(line_1);
  lcd.setCursor(0,1);
  lcd.print(line_2); 
  lcd.write(6);

  last_status_print = millis();
}

void init_servo() {
  servo.write(CLOSED_ANGLE);
}

void feed_fish() {
  play_melody(sound1, 3);

  for(int c = 1; c<= CYCLES_PER_FEED; c++) {

    for(int i = CLOSED_ANGLE; i <= OPEN_ANGLE; i++) {
      servo.write(i);
      delay(3);
    }
  
    for(int i = OPEN_ANGLE; i >= CLOSED_ANGLE; i--) {
      servo.write(i);
      delay(3);
    }

  }

  last_feed = millis();
  inc_num_feeds();

  play_melody(sound2, 8);
}

void set_num_feeds(int feeds) {
  num_feeds = feeds;
}

void inc_num_feeds() {
  set_num_feeds(num_feeds+1);
}

void loop() { 
  if(digitalRead(TRIGGER_PIN) == HIGH) {
    feed_fish();
  }
  
  if(digitalRead(SET_PIN) == HIGH) {
    delay(400);
    if(digitalRead(SET_PIN) == HIGH) {
      delay(1600);
      if(digitalRead(SET_PIN) == HIGH) {
        set_num_feeds(0);
        print_status();
        delay(2000);
      }
    } else {
      auto_enabled = !auto_enabled;
    }
  }

  if(last_feed + FEED_INTERVAL <= millis()) {
    feed_fish();
  }

  print_status();
}
