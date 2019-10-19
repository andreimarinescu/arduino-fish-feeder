void print_boot() {
  lcd.setCursor(1,0);
  lcd.write(7);
  lcd.print("Fish  Feeder");
  lcd.write(7);
  lcd.setCursor(8,1);
  lcd.print("v. "+version_number); 
}

void print_init_wifi() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Connecting WIFI");
}

void print_status() {
  if(last_status_print + 100000 > millis()) {
    return;
  }
  int uptime_hours = millis() / 1000 / 60 / 60;
  String line_1 = "Auto:";
  if(auto_enabled) {
    line_1.concat("ON  Up:");
  } else {
    line_1.concat("OFF  Up:");
  }
  line_1.concat(uptime_hours);
  line_1.concat("h");
  String line_2 = "Feeds: ";
  line_2.concat(num_feeds);
  line_2.concat(" Snd:");
  if(audio_enabled) line_2.concat("ON");
  else line_2.concat("OFF");

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(line_1);
  lcd.setCursor(0,1);
  lcd.print(line_2); 
  // lcd.write(6);

  last_status_print = millis();
}