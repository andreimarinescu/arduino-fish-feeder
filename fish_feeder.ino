#include "constants.h"
#include "hardware.h"
#include "buzzer.h"
#include "wifi.h"
#include "lcd_messages.h"

boolean auto_enabled = true;
unsigned long last_feed = 0;
unsigned long last_status_print = 0;
int num_feeds = 0;

String version_number = "0.3.1";

char ssid[] = WIFI_SSID; // your network SSID (name)
char pass[] = WIFI_PASS; // your network password
int status = WL_IDLE_STATUS; // the Wifi radio's status


void setup() {
  servo.attach(SERVO_PIN);
  pinMode(TRIGGER_PIN, INPUT);
  pinMode(SET_PIN, INPUT);

  lcd.begin(16,2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display } 
  lcd.createChar(LCD_CHAR_HEART, heart);
  lcd.createChar(LCD_CHAR_FISH, fish);

  print_boot();
  init_servo();

  // initialize serial for debugging
  Serial.begin(9600);
  // initialize serial for ESP module
  Serial1.begin(9600);
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
  }

  // attempt to connect to WiFi network
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  // you're connected now, so print out the data
  Serial.println("You're connected to the network");

  //connect to MQTT server
  // client.setServer(MQTT_IP, MQTT_PORT);
  // client.setCallback(callback);

  delay(1000);
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

  // if (!client.connected()) {
  //   reconnect();
  // }
  // client.loop();

  print_status();
}
