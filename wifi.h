//print any message received for subscribed topic
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);

  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    char receivedChar = (char)payload[i];
    Serial.print(receivedChar);
    if (receivedChar == '0')
      Serial.println("Off");
    if (receivedChar == '1')
      Serial.println("On");

  }
  Serial.println();
}

// void reconnect() {
//   // Loop until we're reconnected
//   while (!client.connected()) {
//     Serial.print("Attempting MQTT connection...");
//     // Attempt to connect, just a name to identify the client
//     if (client.connect("NANO", "bpie", "bpass")) {
//       Serial.println("connected");
//       // Once connected, publish an announcement...
//       client.publish("arduino_test","Hello World");
//       // ... and resubscribe
//       client.subscribe("arduino_rcv", 0);

//     } else {
//       Serial.print("failed, rc=");
//       Serial.print(client.state());
//       Serial.println(" try again in 5 seconds");
//       // Wait 5 seconds before retrying
//       delay(5000);
//     }
//   }
// }