#include <Servo.h>
#include <LiquidCrystal.h> // LCD screen driver 
#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include <WiFiEspUdp.h>
#include "SoftwareSerial.h"
#include <PubSubClient.h>

// #include "constants.h"

// Initialize the Ethernet client object
WiFiEspClient espClient;
// PubSubClient client(espClient);
SoftwareSerial Serial1(WIFI_RX, WIFI_TX);

LiquidCrystal lcd(2, 3, 4, 5, 6, 7); // Parameters: (rs, enable, d4, d5, d6, d7) 
Servo servo;
