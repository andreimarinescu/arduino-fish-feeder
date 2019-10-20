// void play_melody(int index, int length) {
//   int *melody;
//   int sound1[3] = {
//     NOTE_C4, NOTE_C4, NOTE_C4
//   };

//   int sound2[8] = {
//     NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
//   };

//   // note durations: 4 = quarter note, 8 = eighth note, etc.:
//   int noteDurations[] = {
//     8, 8, 8, 4, 4, 4, 4, 4
//   };

//   if(!audio_enabled) return;

//   if(index == 1) melody = sound1;
//   else melody = sound2;

//     // iterate over the notes of the melody:
//   for (int thisNote = 0; thisNote < length; thisNote++) {

//     // to calculate the note duration, take one second divided by the note type.
//     //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
//     int noteDuration = 1000 / noteDurations[thisNote];
//     tone(BUZZER_PIN, melody[thisNote], noteDuration);

//     // to distinguish the notes, set a minimum time between them.
//     // the note's duration + 30% seems to work well:
//     int pauseBetweenNotes = noteDuration * 1.30;
//     delay(pauseBetweenNotes);
//     // stop the tone playing:
//     noTone(BUZZER_PIN);
//   }
// }

void buzz(int freq) {
  if(!audio_enabled) return;
  tone(BUZZER_PIN, freq);
  delay(BUZZ_DURATION);
  noTone(BUZZER_PIN);
}