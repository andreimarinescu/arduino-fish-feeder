void setup_wifi() {
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
    Serial.println(WIFI_SSID);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(WIFI_SSID, WIFI_PASS);
  }

  // you're connected now, so print out the data
  Serial.println("You're connected to the network");

  //connect to MQTT server
  client.setServer(MQTT_IP, MQTT_PORT);
  client.setCallback(callback);

}

//print any message received for subscribed topic
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);

  Serial.print("] ");

  char commandArray[10];

  for (int i = 0; i < length; i++) {
    commandArray[i] = (char)payload[i];
  }
  commandArray[length] = '\0';

  String command = String(commandArray);
  Serial.print("Command: ");
  Serial.println(command);
  
  if(command.startsWith("start_feed")) {
    Serial.println("Feeding fish");
    feed_fish();
  } else if(command.startsWith("toggle_audio")) {
    Serial.println("Toggling audio");
    audio_enabled = !audio_enabled;
    print_status(true);
    send_status(true);
  } else if(command.startsWith("reset_feeds")) {
    Serial.println("Resetting feeds");
    set_num_feeds(0);
    print_status(true);
    send_status(true);
  }
}

void send_status() {
  send_status(false);
}

void send_status(boolean override) {
  if((last_status_print + 60000 > millis()) && !override ) {
    return;
  }
  char buffer[110];
  if(client.connected()) {
    String availability_payload = "online";
    availability_payload.toCharArray(buffer, availability_payload.length()+1);
    client.publish("home/feeder_beta/availability", buffer);
    String attributes_payload = " { \"num_feeds\": ";
    attributes_payload.concat(num_feeds);
    attributes_payload.concat(", \"auto_mode\": ");
    if(auto_enabled) attributes_payload.concat("\"on\"");
    else attributes_payload.concat("\"off\"");
    attributes_payload.concat(", \"sound_mode\": ");
    if(audio_enabled) attributes_payload.concat("\"on\"");
    else attributes_payload.concat("\"off\"");
    attributes_payload.concat(", \"firmware\": \"");
    attributes_payload.concat(version_number);
    attributes_payload.concat("\" }");
    attributes_payload.toCharArray(buffer, attributes_payload.length()+1);
    client.publish("home/feeder_beta/attrs", buffer);
    String last_feed_payload = "";
    last_feed_payload.concat(last_feed/1000);
    last_feed_payload.toCharArray(buffer, last_feed_payload.length()+1);
    client.publish("home/feeder_beta/last_feed", buffer);  
    String state_payload = "";
    state_payload.concat(num_feeds);
    state_payload.toCharArray(buffer, state_payload.length()+1);  
    client.publish("home/feeder_beta/state", buffer);
    last_status_print = millis();
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect, just a name to identify the client
    if (client.connect("NANO", "bpie", "bpass")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("home/feeder_beta/messages","Feeder Connected");
      // ... and resubscribe
      client.subscribe("home/feeder_beta/control", 0);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}