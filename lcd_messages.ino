void print_boot() {
  Serial.print(F("Initializing Feeduino v. "));
  Serial.println(VERSION_NUMBER);
  lcd.setCursor(1,0);
  lcd.write(LCD_CHAR_HEART);
  lcd.print(F("Feeduino  SE"));
  lcd.write(LCD_CHAR_HEART);
  lcd.setCursor(8,1);
  lcd.print("v. ");
  lcd.print(VERSION_NUMBER); 
}

void print_init_wifi() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("Connecting WIFI"));
}

void print_status() {
  print_status(false);
}

void print_status(boolean override) {
  if((last_status_print + 60000 > millis()) && !override ) {
    return;
  }
  int uptime_hours = millis() / 1000 / 60 / 60;
  String line_1 = F("Auto:");
  if(auto_enabled) {
    line_1.concat(F("ON  Up:"));
  } else {
    line_1.concat(F("OFF  Up:"));
  }
  line_1.concat(uptime_hours);
  line_1.concat("h");
  String line_2 = F("Feed:");
  line_2.concat(num_feeds);
  line_2.concat(" Snd:");
  if(audio_enabled) line_2.concat("ON");
  else line_2.concat("OFF");

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(line_1);
  lcd.setCursor(0,1);
  lcd.print(line_2); 
  lcd.write(LCD_CHAR_HEART);
}