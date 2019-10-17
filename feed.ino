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