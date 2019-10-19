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
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
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
    print_status();
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
      client.publish("arduino_test","Feeder Connected");
      // ... and resubscribe
      client.subscribe("arduino_rcv", 0);

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}