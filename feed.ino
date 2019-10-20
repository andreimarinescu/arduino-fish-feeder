void init_servo() {
  servo.attach(SERVO_PIN);
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
  delay(100);
  print_status(true);
  send_status(true);

  play_melody(sound2, 8);
}

void set_num_feeds(int feeds) {
  num_feeds = feeds;
  EEPROM.update(0, num_feeds);
  if(client.connected()) {
    String message = "feed_completed @ ";
    message.concat(millis());
    char buffer[50];
    message.toCharArray(buffer, message.length()+1);
    client.publish("home/feeder_beta/messages", buffer);
    client.loop();
  }
}

void inc_num_feeds() {
  set_num_feeds(num_feeds+1);
}