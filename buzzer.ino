void play_melody(int melody[], int length) {
  if(!audio_enabled) return;
    // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < length; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(BUZZER_PIN, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(BUZZER_PIN);
  }
}

void buzz(int freq) {
  if(!audio_enabled) return;
  tone(BUZZER_PIN, freq);
  delay(BUZZ_DURATION);
  noTone(BUZZER_PIN);
}