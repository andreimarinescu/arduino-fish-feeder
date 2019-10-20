void init_servo() {
  servo.attach(SERVO_PIN);
  servo.write(CLOSED_ANGLE);
}

void feed_fish() {
  // play_melody(1, 3);

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

  // play_melody(2, 8);
}

void set_num_feeds(int feeds) {
  num_feeds = feeds;
  if(client.connected()) {
    String message = "feed_completed @ ";
    message.concat(millis());
    char buffer[50];
    message.toCharArray(buffer, message.length());
    client.publish("arduino_test", buffer);
  }
}

void inc_num_feeds() {
  set_num_feeds(num_feeds+1);
}