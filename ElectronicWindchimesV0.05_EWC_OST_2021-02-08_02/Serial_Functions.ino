void readSerial(){
  if (Serial.available() > 0) {
    if (!serialDetected){
      Serial.print("Serial detected\n");
      serialDetected = true;
    }
    serialDetected = true;
    incomingByte = Serial.read();
    switch (incomingByte){
      case 91:
        // Recognize a "["
        Serial.print(F("Printing minimal feedback"));
        Serial.print("\n");
        verboseFeedback = false;
        for (int i=0;i<4;i++){
          updateMoodOnScreen(i);
        }
        break;
      case 93:
        // Recognize a "]"
        Serial.print(F("Printing verbose feedback"));
        Serial.print("\n");
        verboseFeedback = true;
        break;
      case 63:
        // Recognize a "?"
        Serial.print(F("\n\t\tAvailable commands"));
        Serial.print("\n");
        Serial.print(F("?\tDisplay this help menu"));
        Serial.print("\n");
        Serial.print(F("[\tSend minimal feedback"));
        Serial.print("\n");
        Serial.print(F("]\tSend verbose feedback"));
        Serial.print("\n");
        Serial.print(F("S or s\tShow or hide attention sliders"));
        Serial.print("\n");
        break;
      case 115:
        // Recognize "s"
        showSliders = !showSliders;
        Serial.print(F("Slider visibility"));
        Serial.print("\n");
        if(!showSliders){
          for (int i=0;i<4;i++){
            updateMoodOnScreen(i);
          }
        }
        break;
      case 83:
        // Recognize "S"
        showSliders = !showSliders;
        Serial.print(F("Slider visibility"));
        Serial.print("\n");
        if(!showSliders){
          for (int i=0;i<4;i++){
            updateMoodOnScreen(i);
          }
        }
        break;
      default:
        if ((incomingByte == 10) || (incomingByte == 13)){
          break;
        }else{
          Serial.print(F("Your command \""));
          Serial.write(incomingByte);
          Serial.print(F("\" was not a recognized command. Type ? for help.\n"));
        }
        break;
    }
  }
}
