void setup_wifi() {
  // initialize serial for ESP module
  Serial1.begin(9600);
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println(F("WiFi shield not present"));
  }

  // attempt to connect to WiFi network
  while (status != WL_CONNECTED) {
    Serial.print(F("Attempting to connect to WPA SSID: "));
    Serial.println(WIFI_SSID);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(WIFI_SSID, WIFI_PASS);
  }

  // you're connected now, so print out the data
  Serial.println(F("You're connected to the network"));

  //connect to MQTT server
  client.setServer(MQTT_IP, 1883);
  client.setCallback(callback);

}

//print any message received for subscribed topic
void callback(char* topic, byte* payload, unsigned int length) {
  if(last_command_ts + 1000 > millis()) {
    Serial.println(F("MQTT debouncing. Dropping message."));
    return;
  }
  last_command_ts = millis();
  Serial.print(F("Message arrived ["));
  Serial.print(topic);

  Serial.print("] ");

  char commandArray[10];

  for (int i = 0; i < length; i++) {
    commandArray[i] = (char)payload[i];
  }
  commandArray[length] = '\0';

  String command = String(commandArray);
  Serial.print(F("Command: "));
  Serial.println(command);
  
  if(command.startsWith("start_feed")) {
    Serial.println(F("Feeding fish"));
    feed_fish();
  } else if(command.startsWith("toggle_auto")) {
    Serial.println(F("Toggling auto"));
    auto_enabled = !auto_enabled;
    client.loop();
    print_status(true);
    send_status(true);
    client.loop();
  } else if(command.startsWith("toggle_audio")) {
    Serial.println(F("Toggling audio"));
    audio_enabled = !audio_enabled;
    client.loop();
    print_status(true);
    send_status(true);
    client.loop();
  } else if(command.startsWith("reset_feeds")) {
    Serial.println(F("Resetting feeds"));
    client.loop();
    set_num_feeds(0);
    print_status(true);
    send_status(true);
    client.loop();
  }
}

void send_status() {
  send_status(false);
}

void send_status(boolean override) {
  if((last_status_print + 60000 > millis()) && !override ) {
    return;
  }
  if(client.connected()) {
    Serial.println(F("Publishing status"));
    send_availability();
    delay(100);
    send_attributes();
    delay(200);
    send_state();
    delay(100);
    send_last_feed();
    last_status_print = millis();
    Serial.println(F("Finished publishing status"));
  }
}


void send_availability() {
  char buffer[15];
  String availability_payload = F("online");
  availability_payload.toCharArray(buffer, availability_payload.length()+1);
  client.publish("home/feeder_beta/availability", buffer);
}

void send_attributes() {
  char buffer[110];
  String attributes_payload = F(" { \"num_feeds\": ");
  attributes_payload.concat(num_feeds);
  attributes_payload.concat(F(", \"auto_mode\": "));
  if(auto_enabled) attributes_payload.concat("\"on\"");
  else attributes_payload.concat(F("\"off\""));
  attributes_payload.concat(F(", \"sound_mode\": "));
  if(audio_enabled) attributes_payload.concat("\"on\"");
  else attributes_payload.concat(F("\"off\""));
  attributes_payload.concat(F(", \"firmware\": \""));
  attributes_payload.concat(VERSION_NUMBER);
  attributes_payload.concat(F("\" }"));
  attributes_payload.toCharArray(buffer, attributes_payload.length()+1);
  client.publish("home/feeder_beta/attrs", buffer);
}

void send_last_feed() {
  char buffer[30];
  String last_feed_payload = "";
  last_feed_payload.concat(last_feed/1000);
  last_feed_payload.toCharArray(buffer, last_feed_payload.length()+1);
  client.publish("home/feeder_beta/last_feed", buffer);  
}

void send_state() {
  char buffer[10];
  String state_payload = "";
  state_payload.concat(num_feeds);
  state_payload.toCharArray(buffer, state_payload.length()+1);  
  client.publish("home/feeder_beta/state", buffer);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print(F("Attempting MQTT connection..."));
    // Attempt to connect, just a name to identify the client
    if (client.connect("NANO", MQTT_USER, MQTT_PASS)) {
      Serial.println(F("connected"));
      // Once connected, publish an announcement...
      client.publish("home/feeder_beta/messages","Feeder Connected");
      // ... and resubscribe
      client.subscribe("home/feeder_beta/control", 0);
    } else {
      Serial.print(F("failed, rc="));
      Serial.print(client.state());
      Serial.println(F(" try again in 5 seconds"));
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}