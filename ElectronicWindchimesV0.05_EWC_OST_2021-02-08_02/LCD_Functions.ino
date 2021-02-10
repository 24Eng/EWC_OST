void splashScreen(){
  myGLCD.fillScr(0, 0, 255);
  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRoundRect(160, 70, 319, 169);
  
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 0, 0);
  myGLCD.setFont(BigFont);
  myGLCD.print("EWC_OST", CENTER, 93);
  myGLCD.setFont(SmallFont);
  myGLCD.print("24HourEngineer", CENTER, 119);
  myGLCD.print("Brian McEvoy", CENTER, 132);
//  
//  myGLCD.setColor(0, 255, 0);
//  myGLCD.setBackColor(0, 0, 255);
//  myGLCD.print("Runtime: (msecs)", CENTER, 290);
//  myGLCD.printNumI(millis(), CENTER, 305);
}

void panicScreen(){
  int funTextBGR = 255;
  int funTextBGG = 255;
  int funTextBGB = 0;
  myGLCD.fillScr(255, 0, 0);
  myGLCD.setColor(funTextBGR, funTextBGG, funTextBGB);
//  myGLCD.fillRoundRect(160, 70, 319, 169);
  myGLCD.fillRoundRect(240-80, 160-50, 240+80, 160+50);
  
  myGLCD.setColor(0, 0, 0);
  myGLCD.setBackColor(funTextBGR, funTextBGG, funTextBGB);
  myGLCD.setFont(BigFont);
  myGLCD.print("PANIC", CENTER, 140);
  myGLCD.setFont(SmallFont);
  myGLCD.print("Twist to release", CENTER, 180);
}

void cardNotFound(){
  myGLCD.fillScr(100, 0, 0);
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRoundRect(160, 70, 319, 169);
  
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 0, 0);
  myGLCD.setFont(BigFont);
  myGLCD.print("EWC_OST", CENTER, 93);
  myGLCD.setFont(SmallFont);
  myGLCD.print("SD card", CENTER, 119);
  myGLCD.print("not found", CENTER, 132);
  
  myGLCD.setColor(0, 255, 0);
  myGLCD.setBackColor(0, 0, 255);
  myGLCD.print("Runtime: (msecs)", CENTER, 290);
  myGLCD.printNumI(millis(), CENTER, 305);
}

void clearScreen(){
  myGLCD.fillScr(100, 100, 100);
}


void updateMoodOnScreen(int funMood){
  int printXOffset = 0;
  int textPoint = 0;
  int funInstrumentParameters = 6;
  myGLCD.setColor(moods[funMood][2], moods[funMood][3], moods[funMood][4]);
  if(funMood == 0){
    myGLCD.fillRect(2, 2, 240, 159);
    printXOffset = 15;
    textPoint = 25;
  }
  if(funMood == 1){
    myGLCD.fillRect(241, 2, 480, 159);
    printXOffset = 260;
    textPoint = 25;
  }
  if(funMood == 2){
    myGLCD.fillRect(2, 160, 240, 320);
    printXOffset = 15;
    textPoint = 190;
  }
  if(funMood == 3){
    myGLCD.fillRect(241, 160, 480, 320);
    printXOffset = 260;
    textPoint = 190;
  }
  
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 0, 0);
  int incrementMovement = 25;
  myGLCD.setFont(BigFont);
  String funPrintableName = "";
  for (int i=0; i<32; i++){
    funPrintableName += moodName[funMood][i];
  }
  myGLCD.print(funPrintableName, printXOffset, textPoint-20);
  myGLCD.setFont(SmallFont);

  for (int i=1; i<6; i++){
    if((moods[funMood][(i*funInstrumentParameters+3)] > 0) && (moods[funMood][(i*funInstrumentParameters+1)] > 0)){
      strcpy_P(buffer, (char *)pgm_read_word(&(MIDIList[moods[funMood][(i*funInstrumentParameters)]])));  // Necessary casts and dereferencing, just copy.
      myGLCD.print(buffer, printXOffset, textPoint + (i*incrementMovement - incrementMovement));
      
      if(moods[funMood][(i*6+2)]){
        myGLCD.setBackColor(30, 30, 30);
      }
      myGLCD.print("%: ", printXOffset+10, textPoint + (i*incrementMovement - (incrementMovement/2)-1));
      myGLCD.printNumI(moods[funMood][(i*funInstrumentParameters+1)], printXOffset+25, textPoint + (i*incrementMovement - (incrementMovement/2)-1));
      
      if(moods[funMood][(i*funInstrumentParameters+4)]){
        myGLCD.setBackColor(30, 30, 30);
      }else{
        myGLCD.setBackColor(0, 0, 0);
      }
      myGLCD.print("V: ", printXOffset+55, textPoint + (i*incrementMovement - (incrementMovement/2)-1));
      myGLCD.printNumI(moods[funMood][(i*funInstrumentParameters+3)], printXOffset+70, textPoint + (i*incrementMovement - (incrementMovement/2)-1));
      
      myGLCD.setBackColor(0, 0, 0);
      myGLCD.print("O: ", printXOffset+100, textPoint + (i*incrementMovement - (incrementMovement/2)-1));
      myGLCD.printNumI(moods[funMood][(i*funInstrumentParameters+5)], printXOffset+115, textPoint + (i*incrementMovement - (incrementMovement/2)-1));
    }else{
      myGLCD.print("     Silent     ", printXOffset, textPoint + (i*incrementMovement - incrementMovement));
    }
  }
}

void onScreenPercentage(int funQuadrant, int funPercentage){
  int funBGXStart = 210;
  int funBGYStart = 40;
  int funBGXEnd = 0;
  int funBGYEnd = 0;
  int funGraphWidth = 2;
  int funGraphHeight = 100;
  
  if(funQuadrant == 0){
    
  }
  if(funQuadrant == 1){
    funBGXStart = (240 + funBGXStart);
  }
  if(funQuadrant == 2){
    funBGYStart = (160 + funBGYStart);
  }
  if(funQuadrant == 3){
    funBGXStart = (240 + funBGXStart);
    funBGYStart = (160 + funBGYStart);
  }
  funBGXEnd = funBGXStart + funGraphWidth;
  funBGYEnd = funBGYStart + funGraphHeight;
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRect(funBGXStart, funBGYStart, funBGXEnd, funBGYStart+(100-funPercentage));
  myGLCD.setColor(255, 255, 255);
  myGLCD.fillRect(funBGXStart, funBGYStart+(100-funPercentage), funBGXEnd, funBGYEnd);
}

void drawHorizontalSlider(){
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRect(0, 0, 480, 1);
  myGLCD.setColor(255, 255, 255);
  int funMappedPosition = map(analogValues[0], 0, 1023, 0, 475);
  myGLCD.fillRect(funMappedPosition, 0, funMappedPosition+5, 1);
 }

void drawVerticalSlider(){
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRect(0, 0, 1, 320);
  myGLCD.setColor(255, 255, 255);
  int funMappedPosition = map(analogValues[1], 0, 1023, 315, 0);
  myGLCD.fillRect(0, funMappedPosition, 1, funMappedPosition+5);
}
