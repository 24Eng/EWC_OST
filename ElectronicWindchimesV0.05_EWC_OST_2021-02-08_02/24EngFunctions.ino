// Ensure that the note about to be played is within the limits.
int maintainNoteLimits(int funStartingPoint, int funRequestedChange, int funLowerLimit, int funUpperLimit){
  int safelyRangedChange;
  boolean unaltered = true;
  int requestedNote = funStartingPoint + minorPentatonicScale[funRequestedChange];
  // Assess if the requested note needs to be raised to keep it above the lower threshold.
  if (requestedNote < funLowerLimit){
    // If the requested change is already greater than six, the limits were probably
    // changed recently so just keep going to a higher note. Else, add six to make the change positive.
    if (funRequestedChange > 6){
      safelyRangedChange = funRequestedChange;
      unaltered = false;
    }else{
      safelyRangedChange = 6 + funRequestedChange;
      unaltered = false;
    }
  }
  // Assess if the requested note needs to be lowered to keep it below the upper threshold.
  if ((requestedNote > funUpperLimit) && unaltered){
    // If the requested change is already less than six, the limits were probably
    // changed recently so just keep going to a lower note. Else, subtract six to make the change negative.
    if (funRequestedChange < 6){
      safelyRangedChange = funRequestedChange;
    }else{
      safelyRangedChange = funRequestedChange - 6;
    }
  }
  // For an unknown reason, the next print statement must be intact for
  // the code to run properly.
  // DO NOT DELETE THIS PRINTLN STATEMENT.
  // But sometimes deleting it works too. YMMV
//  Serial.print("");
//  Serial.println(safelyRangedChange);
  // For an unknown reason, the previous print statement must be intact for
  // the code to run properly.
  // DO NOT DELETE THIS PRINTLN STATEMENT
  // But sometimes deleting it works too. YMMV
  return safelyRangedChange;
}

void MIDICommand(int statusByte, int data1Byte, int data2Byte){
  Serial2.write(statusByte);
  Serial2.write(data1Byte);
  Serial2.write(data2Byte);
}



void calculateMainNote(){
  // Generate the next note.
  noteDelta = random(0, 12);
  // Check to see that the note will be within the prescribed range.
  noteDelta = maintainNoteLimits(mainNote, noteDelta, lowerThreshold, upperThreshold);
  // Change the mainNote variable according to the selected scale.
  mainNote = mainNote + minorPentatonicScale[noteDelta];
  nextNoteIteration = timeThisCycle + (mainTempo);
}

void calculateChannel00(){
  // Channel zero always has a time offset of zero, a pitch offset of
  // zero, and plays at full volume so it is calculated differently.
  int chanceToPlay = random(0, 100);
  if (mainBPM == minTempo){
    chanceToPlay = 100;
  }
  if (chanceToPlay < channelTweaks[0][2]){
    primaryNotes[0][0][0] = mainNote;
    primaryNotes[0][1][0] = timeThisCycle;
    primaryNotes[0][2][0] = timeThisCycle + mainTempo;
    primaryNotes[0][3][0] = channelTweaks[0][4];
    primaryNotes[0][4][0] = 1;
  }
}

void calculateChannels(boolean funEvenOdd){
//  Serial.print("\n");
  for (int i=1; i <= activeChannels; i++){
    if (i != 9){
      // Calculate the Pitch offset
      primaryNotes[i][0][funEvenOdd] = mainNote + channelTweaks[i][0];
      // Calculate the Time on value
      long funTimeOffset = (channelTweaks[i][1] * mainTempo) / 10;
      long funTimeOn = timeThisCycle + funTimeOffset;
      primaryNotes[i][1][funEvenOdd] = funTimeOn;
      // Calculate the Time off value
      long funDuration =  (channelTweaks[i][3]*mainTempo) / 10;
      primaryNotes[i][2][funEvenOdd] = funTimeOn + funDuration;
      // Calculate the note Velocity
      primaryNotes[i][3][funEvenOdd] = channelTweaks[i][4];
      // Set value [4] to one so it can begin playing
      primaryNotes[i][4][funEvenOdd] = 1;
//      Serial.print("Channel: ");
//      Serial.print(i);
//      Serial.print("\t");
//      for (int e=0; e<5; e++){
//        Serial.print(primaryNotes[i][e][funEvenOdd]);
//        Serial.print(", ");
//      }
//      Serial.print("\n");
    }
  }
}


void printChannelInformation(){
  Serial.print("\n");
  Serial.print(F("* * * * * Channel info * * * * *"));
  Serial.print("\n");
  for (int i = 0; i<=activeChannels; i++){
    if (i == 9){
      i = 10;
    }
    Serial.print(F("Channel: "));
    Serial.print(i);
    Serial.print(F("\tInstrument: "));
    Serial.print(channelTweaks[i][5]);
    Serial.print(F("\tPitch offset: "));
    Serial.print(channelTweaks[i][0]);
    Serial.print(F(" \tTime offset: "));
    Serial.print(channelTweaks[i][1]);
    Serial.print(F("\tPlay frequency: "));
    Serial.print(channelTweaks[i][2]);
    Serial.print(F("\tVelocity: "));
    Serial.print(channelTweaks[i][4]);
    Serial.print("\n");
  }
  Serial.print(F("Mode: "));
  Serial.print(mode);
  Serial.print(F("\tIntensity: "));
  Serial.print(mainIntensity);
  Serial.print("\n");
}

void panic(){
  for (int e=0; e <= 1; e++){
    for (int i = 0; i < 16; i++){
      for (int k = 0; k < 127; k++){
        midiNoteOff(i, k, 0);
      }
      midiSetChannelVolume(i, 127);
      primaryNotes[i][4][e] = 0;
    }
  }
  activeChannels = 0;
  if (serialDetected){
      Serial.print(F("Panic"));
      Serial.print("\n");
  }
//  delay(1000);
}

void stopAllNotes(){
  for (int e=0; e <= 1; e++){
    for (int i = 0; i < 16; i++){
      for (int k = 0; k < 127; k++){
        midiNoteOff(i, k, 0);
      }
      primaryNotes[i][4][e] = 0;
    }
  }
}

void stopActiveNotes(){
  for (int e=0; e <= 1; e++){
    for (int i = 0; i <= activeChannels; i++){
      for (int k = 0; k < 127; k++){
        midiNoteOff(i, k, 0);
      }
      primaryNotes[i][4][e] = 0;
    }
  }
}

void printMIDIToTerminal(uint8_t chan, uint8_t n, uint8_t vel){
  Serial.print("\n");
  Serial.print(chan, HEX);
  Serial.print(", ");
  Serial.print(n);
  Serial.print(", ");
  Serial.print(vel);
}

void remainCalm(){
  if((millis() < 2000) && !buttonStates[panicButton]){
    panicScreen();
    panic();
  }
  if(risingEdges[panicButton]){
    panicScreen();
    panic();
  }
  if(fallingEdges[panicButton]){
    for(int i=0; i<4; i++){
      updateMoodOnScreen(i);
    }
    checkAnalogInputs();
  }
}

void checkDiscreteInputs(){
  for (int i=0; i<9; i++){
    buttonStates[i] = digitalRead(i+2);
    if(buttonStates[i] < oldButtonStates[i]){
      risingEdges[i] = true;
    }else{
      risingEdges[i] = false;
    }
    if(buttonStates[i] > oldButtonStates[i]){
      fallingEdges[i] = true;
    }else{
      fallingEdges[i] = false;
    }
    oldButtonStates[i] = buttonStates[i];
  }
}

void checkAnalogInputs(){
  for(int i=0; i<2; i++){
    analogValues[i] = analogRead(i);
    if ((analogValues[i] - oldAnalogValues[i]) >= analogDrift){
      analogIncreasing[i] = true;
      oldAnalogValues[i] = analogValues[i];
    }else{
      analogIncreasing[i] = false;
    }
    if ((oldAnalogValues[i] - analogValues[i] >= analogDrift)){
      analogDecreasing[i] = true;
      oldAnalogValues[i] = analogValues[i];
    }else{
      analogDecreasing[i] = false;
    }
  }
  // Redraw lines on the screen if there is a change
  if(analogDecreasing[0] || analogIncreasing[0]){
    drawHorizontalSlider();
  }
  if(analogDecreasing[1] || analogIncreasing[1]){
    drawVerticalSlider();
  }
  if(analogDecreasing[0] || analogIncreasing[0] || analogDecreasing[1] || analogIncreasing[1]){
    analogChange = true;
  }else{
    analogChange = false;
  }
}

void maintainMoods(){
  bool funMoodChange = false;
  for (int i=0;i<4;i++){
    if(risingEdges[moodIncrementRoster[i]]){
      moods[i][0]++;
      funMoodChange = true;
    }
    if(moods[i][0] >= availableMoods){
      moods[i][0] = 0;
    }
    if(risingEdges[moodDecrementRoster[i]]){
      moods[i][0]--;
      funMoodChange = true;
    }
    if(moods[i][0] < 0){
      moods[i][0] = availableMoods-1;
    }
    if(funMoodChange){
      if(verboseFeedback){
        Serial.print("Mood ");
        Serial.write(i+65);
        Serial.print(" change: ");
        Serial.print(moods[i][0]);
        Serial.print("\n");
      }
      MIDIChannelNotesOff(i);
      updateMoodParameters(i);
      updateMoodOnScreen(i);
      updateAttentionPercentages();
      funMoodChange = false;
    }
  }
}

//void printInputs(int funX, int funY){
//  if (funX==9 && funY==9){
//    delay(1);
//  }else{
//    Serial.print("x: ");
//    Serial.print(funX);
//    Serial.print("\ty: ");
//    Serial.print(funY);
//    Serial.print("\t\tTime:\t");
//    Serial.println(millis());
//  }
//}
//
//void printInputs(){
//  Serial.print("BPM: ");
//  Serial.print(mainBPM);
//  Serial.print("\t");
//  Serial.print("Velocity: ");
//  Serial.print(mainVelocity);
//  Serial.print("\t");
//  Serial.print(randomizeChannelInput);
//  Serial.print("\t");
//  Serial.print(confirmChannelInput);
//  Serial.print("\t");
//  Serial.print(manualPlayInput);
//  Serial.print("\n");
//}


int attentionPercentage(int funQuadrant, int funSlider1, int funSlider2){
  funSlider1 = map(funSlider1, 0, 1023, 0, 100);
  funSlider2 = map(funSlider2, 0, 1023, 0, 100);
  if(funQuadrant == 0){
    funSlider1 = 100 - funSlider1;
  }
  if(funQuadrant == 3){
    funSlider2 = 100 - funSlider2;
  }
  if(funQuadrant == 2){
    funSlider1 = 100 - funSlider1;
    funSlider2 = 100 - funSlider2;
  }
  int funSliderProduct = funSlider1 * funSlider2;
  int funSliderPercent = funSliderProduct / 100;
  return(funSliderPercent);
}

void calculateTempo(){
  int funRunningTotal = 0;
  if(verboseFeedback){
    Serial.print("BPM: ");
  }
  for(int i=0; i<4; i++){
    int funTempoTimesAttention = (moods[i][5] * moods[i][1]);
    funRunningTotal = funRunningTotal + funTempoTimesAttention;
    if(verboseFeedback){
      Serial.print(funTempoTimesAttention);
      Serial.print(", ");
    }
  }
  funRunningTotal = funRunningTotal / 100;
  if(verboseFeedback){
    Serial.print("\t Total: ");
    Serial.print(funRunningTotal);
    Serial.print("\n");
  }
  mainBPM = constrain(funRunningTotal, minTempo, maxTempo);
  mainTempo = 60000 / mainBPM;
}

void updateAttentionPercentages(){
  int percentageTotal = 0;
  int newestPercentage = 0;
  for (int i=0; i<4; i++){
    newestPercentage = attentionPercentage(i, analogValues[0], analogValues[1]);
    percentageTotal = percentageTotal + newestPercentage;
    if(showSliders){
      onScreenPercentage(i, newestPercentage);
    }
    moods[i][1] = newestPercentage;
  }
  calculateTempo();
}

void playMoods(){
  MIDINotesOff();
  calculateMainNote();
  for (int i=0;i<4;i++){
    for(int k=6; k<=30;k=k+funInstrumentOffset){
      if((moods[i][k+1] > 0) && (moods[i][k+3] > 0)){
        midiSetInstrument(i, moods[i][k]);
        bool probability = calculateProbability(moods[i][k+1], moods[i][1]);
        if (probability){
          int octaveModifier = moods[i][k+5];
          int temperedVelocity = moods[i][k+3];
          if (moods[i][k+4]){
            temperedVelocity = calculateVelocity(moods[i][k+3], moods[i][1]);
          }
          midiNoteOn(i, mainNote + octaveModifier, temperedVelocity);
        }
      }
    }
  }
}

void MIDINotesOff(){
  for (int i=0;i<4;i++){
    for(int k=6; k<=30;k=k+funInstrumentOffset){
      int octaveModifier = moods[i][k+5];
      midiNoteOff(i, mainNote + octaveModifier, 0);
    }
  }
}

void MIDIChannelNotesOff(int funChannel){
  for(int k=6; k<=30;k=k+funInstrumentOffset){
    int octaveModifier = moods[funChannel][k+5];
    midiNoteOff(funChannel, mainNote + octaveModifier, 0);
  }
}

bool calculateProbability(int fullLikelihood, int attentionPercentage){
  float floatStart = fullLikelihood / 100.0;
  float floatPercent = attentionPercentage / 100.0;
  float floatPrandom = random(0, 100) / 100.0;
  if(floatPrandom < (floatStart * floatPercent)){
    return true;
  }
  return false;
}

int calculateVelocity(int fullVelocity, int attentionPercentage){
  float floatVelocity = fullVelocity * 1.0;
  float floatAttn = attentionPercentage / 100.0;
  float modifiedVelocity = floatVelocity * floatAttn;
  int integerVelocity = (int) modifiedVelocity;
  return integerVelocity;
}
