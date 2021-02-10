

void SDCardStart(){
  boolean readOneShot = true;
  // open the file for reading:
  myFile = SD.open("moods.csv");
  if (myFile) {
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      int newRead = myFile.read();
      // After the first newline, record the first four rows to the moods[][] array
      if((newRead == 13) && (readOneShot)){
        integerFromSerial();
        for(int i=0; i<4; i++){
          for (int k=0; k<moodVariables+1; k++){
            if (k==1){
              charFromSerial(i);
              if (serialDetected){
                Serial.print("Mood ");
                Serial.print(i);
                Serial.print(" name: ");
                for(int m = 0; m < 32; m++){
                  Serial.print(moodName[i][m]);
                }
                Serial.print("\n");
              }
            }else{
              newRead = integerFromSerial();
            }
            moods[i][k] = newRead;
          }
        }
        readOneShot = false;
      }
      if(newRead == 13){
        availableMoods++;
      }
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.print("\nError opening moods.csv");
    Serial.print("\n");
  }
}


void updateMoodParameters(int funMood){
  // This function assumes that the first parameter of the moods[] array 
  // is already changed to the desired mood on the card.
  int funDesiredMood = moods[funMood][0];
  bool funOneShot = true;
  // Erase the mood name
  for (int i=0; i<32; i++){
    moodName[funMood][i] = 0;
  }
  
  // open the file for reading:
  myFile = SD.open("moods.csv");
  if (myFile) {
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      int newRead = myFile.read();
      // After the first newline, record the first four rows to the moods[][] array
      if (newRead == 13 && funOneShot){
        newRead = integerFromSerial();
        newRead = integerFromSerial();
        if(newRead == funDesiredMood){
          for (int k=1; k<moodVariables; k++){
            if (k==1){
              charFromSerial(funMood);
            }else{
              newRead = integerFromSerial();
              moods[funMood][k] = newRead;
            }
            
          }
          funOneShot = false;
        }
      }
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.print("\nError opening moods.csv");
    Serial.print("\n");
    SD.begin(53);
  }
  if (verboseFeedback){
    reportMoods();
  }
}

int integerFromSerial(){
  // This function assumes we've encountered a comma delimiter and the next reading from myFile.read() will be 
  // the start of a number. It will continue reading until it encounters another comma. If it
  // sees a newline (13) or comma (44) right away, it returns 9999
  int funRead = 0;
  int funRollingTotal = 0;
  bool funNegativeInteger = false;
  funRead = myFile.read();
  if ((funRead == 44) || (funRead == 13)){
    return;
  }
  // If the ingeger is negative, the minus symbol will come first. In that case,
  // set a bit high and multiply the result at the end.
  if (funRead == 45){
    funNegativeInteger = true;
    funRead = myFile.read();
  }
  while (funRead>=48 && funRead<58){
    funRollingTotal = funRollingTotal * 10;
    funRollingTotal = funRollingTotal + (funRead - 48);
    funRead = myFile.read();
  }
  while((funRead >= 65) && (funRead <= 90) || (funRead == 32) || (funRead >= 97) && (funRead <= 122)){
    funRead = myFile.read();
  }
  if(funNegativeInteger){
    funRollingTotal = (-1) * funRollingTotal;
  }
  return funRollingTotal;
}

char charFromSerial(int funIteration){
  // This function assumes we've encountered a comma delimiter and the next reading from myFile.read() will be 
  // the start of a character string. It will continue reading until it encounters another comma. If it
  // sees a newline (13) or comma (44) right away, it returns 9999
  int funRead = 0;
  int funRollingTotal = 0;
  int funCharacterCounter = 0;
  funRead = myFile.read();
  if ((funRead == 44) || (funRead == 13)){
    return;
  }
  while (funRead>=48 && funRead<58){
    funRead = myFile.read();
  }
  while((funRead >= 65) && (funRead <= 90) || (funRead == 32) || (funRead >= 97) && (funRead <= 122)){
    moodName[funIteration][funCharacterCounter] = funRead;
    funCharacterCounter++;
    funRead = myFile.read();
  }
  return moodName[funIteration][funCharacterCounter];
}

void reportMoods(){
  Serial.print("Mood array\n");
  for (int i=0; i<4; i++){
    for (int k=0; k<moodVariables; k++){
      Serial.print(moods[i][k]);
      Serial.print("\t");
    }
    Serial.print("\n");
  }
  Serial.print("\n");
}
