#include <PWMServo.h>
#include <LiquidCrystal.h> // LCD screen driver 
#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include <WiFiEspUdp.h>
#include "SoftwareSerial.h"
#include <PubSubClient.h>
#include "constants.h"

char ssid[]                       = WIFI_SSID;
char pass[]                       = WIFI_PASS;
int status                        = WL_IDLE_STATUS; // the Wifi radio's status
int num_feeds                     = 0;
boolean auto_enabled              = true;
boolean audio_enabled             = false;
String version_number             = VERSION_NUMBER;
unsigned long last_feed           = 0;
unsigned long last_status_print   = 0;

PWMServo servo;
WiFiEspClient espClient;
PubSubClient client(espClient);
SoftwareSerial Serial1(WIFI_RX, WIFI_TX);
LiquidCrystal lcd(2, 3, 4, 5, 6, 7); // Parameters: (rs, enable, d4, d5, d6, d7) 

void setup() {
  pinMode(TRIGGER_PIN, INPUT);
  pinMode(SET_PIN, INPUT);

  lcd.begin(16,2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display } 
  lcd.createChar(LCD_CHAR_HEART, heart);
  lcd.createChar(LCD_CHAR_FISH, fish);

  print_boot();
  delay(1500);
  init_servo();
  print_init_wifi();
  setup_wifi();
  print_status();
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

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  print_status();
}
