#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <PubSubClient.h>
#include <PWMServo.h>
#include "SoftwareSerial.h"
#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include <WiFiEspUdp.h>
#include "constants.h"

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
  lcd.begin(16,2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display } 
  
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
  lcd.createChar(LCD_CHAR_HEART, heart);
  // lcd.createChar(LCD_CHAR_FISH, fish);

  print_boot();
  num_feeds = (int) EEPROM.read(0);
  delay(1500);
  init_servo();
  print_init_wifi();
  setup_wifi();
  print_status(true);
  send_status(true);
}

void loop() { 
  if(last_feed + FEED_INTERVAL <= millis()) {
    feed_fish();
  }

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  print_status();
  send_status();
}
