void actOnMode00(){
  // Scan the primaryNotes array for any notes which are ready to
  // play or stop. 
  for (int e=0; e <= 1; e++){
    for (int i=0; i <= activeChannels; i++){
      if (primaryNotes[i][4][e] > 0){
        // If the fourth value is two, that means the note
        // wants to stop when the time arrives.
        if ((primaryNotes[i][4][e] == 2) && (timeThisCycle >= primaryNotes[i][2][e])){
          midiNoteOff(i, primaryNotes[i][0][e], 0);
          primaryNotes[i][4][e] = 0;
        }
        // If the fourth value is one, that means the note
        // wants to play when the time arrives.
        if ((primaryNotes[i][4][e] == 1) && (timeThisCycle >= primaryNotes[i][1][e])){
          midiNoteOn(i, primaryNotes[i][0][e], primaryNotes[i][3][e]);
//          Serial.print("\nNote: ");
//          Serial.print(i);
//          Serial.print(", ");
//          Serial.print(primaryNotes[i][0][e]);
//          Serial.print(", ");
//          Serial.println(primaryNotes[i][3][e]);
          primaryNotes[i][4][e] = 2;
        }
      }
    }
  }
  // Generate the mainNote which will seed the rest of the playable notes
  if (timeThisCycle >= nextNoteIteration){
    calculateMainNote();
    calculateChannel00();
    calculateChannels(evenOddNotePlaying);
    evenOddNotePlaying = !evenOddNotePlaying;
  }
}

void actOnMode01(){
  
  // Generate the mainNote which will seed the rest of the playable notes
  if (timeThisCycle >= nextNoteIteration){
    
    playMoods();
  }
}
