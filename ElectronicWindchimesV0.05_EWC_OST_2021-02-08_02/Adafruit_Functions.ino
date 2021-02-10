// The following are functions from Adafruit
void midiSetInstrument(uint8_t chan, uint8_t inst) {
  if (chan > 15) return;
//  inst --; // page 32 has instruments starting with 1 not 0 :(
  if (inst > 127) return;
  
  Serial2.write(MIDI_CHAN_PROGRAM | chan);  
  delay(10);
  Serial2.write(inst);
  delay(10);
//  Serial.print("Instrument: ");
//  Serial.print(chan);
//  Serial.print(", ");
//  Serial.println(inst);
}
void midiSetChannelVolume(uint8_t chan, uint8_t vol) {
  if (chan > 15) return;
  if (vol > 127) return;
  
  Serial2.write(MIDI_CHAN_MSG | chan);
  Serial2.write(MIDI_CHAN_VOLUME);
  Serial2.write(vol);
}
void midiSetChannelBank(uint8_t chan, uint8_t bank) {
  if (chan > 15) return;
  if (bank > 127) return;
  
  Serial2.write(MIDI_CHAN_MSG | chan);
  Serial2.write((uint8_t)MIDI_CHAN_BANK);
  Serial2.write(bank);
}
void midiNoteOn(uint8_t chan, uint8_t n, uint8_t vel) {
  if (chan > 15){
    Serial.print("Channel out of range\n");
    return;
  }
  if (n > 127){
    Serial.print("Note out of range\n");
    return;
  }
  if (vel > 127){
    Serial.print("Velocity out of range\n");
    return;
  }
  
  Serial2.write(MIDI_NOTE_ON | chan);
  Serial2.write(n);
  Serial2.write(vel);
//  printMIDIToTerminal(chan, n, vel);
//  Serial.print("\n");
}
void midiNoteOff(uint8_t chan, uint8_t n, uint8_t vel) {
  if (chan > 15) return;
  if (n > 127) return;
  if (vel > 127) return;
  
  Serial2.write(MIDI_NOTE_OFF | chan);
  Serial2.write(n);
  Serial2.write(vel);
}
