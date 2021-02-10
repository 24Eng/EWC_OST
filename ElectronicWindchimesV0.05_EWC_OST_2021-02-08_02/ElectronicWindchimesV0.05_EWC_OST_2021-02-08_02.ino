// http://www.rinkydinkelectronics.com/library.php?id=51
// This program requires the UTFT library.

#include <UTFT.h>
// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
UTFT myGLCD(ILI9486,38,39,40,41);

// SD card reader code
#include <SPI.h>
#include <SD.h>
File myFile;

// define the pins used
#define VS1053_RX 2

// If you're using the Music Maker shield:
// Don't forget to connect the GPIO #1 pin to 3.3V and the RX pin to digital #2

// Constants from Adafruit code
#define MIDI_NOTE_ON  0x90
#define MIDI_NOTE_OFF 0x80
#define MIDI_CHAN_MSG 0xB0
#define MIDI_CHAN_BANK 0x00
#define MIDI_CHAN_VOLUME 0x07
#define MIDI_CHAN_PROGRAM 0xC0


// Define the inputs
// The panic and mood designators below are not pin numbers.
// Their buttons are all connected to pins 2 higher than the number shown.
// For example, the button to connect for moodAIncrement is 7.
int moodAIncrement = 5;
int moodADecrement = 6;
int moodBIncrement = 7;
int moodBDecrement = 8;
int moodCIncrement = 1;
int moodCDecrement = 2;
int moodDIncrement = 3;
int moodDDecrement = 4;
int panicButton = 0;
int moodIncrementRoster[] = {moodAIncrement, moodBIncrement, moodCIncrement, moodDIncrement};
int moodDecrementRoster[] = {moodADecrement, moodBDecrement, moodCDecrement, moodDDecrement};

int onboardLED = 13;
int verticalPotentiometer = 0;
int horizontalPotentiometer = 0;
int mode = 0;
int numberOfModes = 4;  // Zero justified.
int playingFanfare = 0;
byte fanfareSequence[] = {127,3,1,0,2,4,7,9};


// Create some useful variables
int currentNote = 60;
int maxTempo = 600;
int minTempo = 30;
int activeChannels = 0;
int runningChannels = 0;
int lowerThreshold = 48;
int upperThreshold = 72;
int funInstrumentOffset = 6;

bool buttonStates[9];
bool oldButtonStates[9];
bool risingEdges[9];
bool fallingEdges[9];
int analogValues[2];
int oldAnalogValues[2];
int analogDrift = 2;
bool analogIncreasing[2];
bool analogDecreasing[2];
bool analogChange = false;
bool serialDetected = false;

int availableMoods = 4;


// Copy over some variables from old code
int noteDelta = 0;
int mainNote = 60;
int mainVelocity = 70;
int mainTempo = 500;
int mainBPM = 120;
int mainIntensity = 0;
int oldMainIntensity = 0;
long nextNoteIteration = 0;
int mainChannel = 0;
long timeThisCycle;
int minIntensity = 0;
int maxIntensity = 299;
boolean updateIntensity = false;
int intensityBreaks = 6;
int intensityBreakpoint = (maxIntensity / intensityBreaks) + 1;
int BGMSet = 0;
int totalBGMs = 1;  //Zero justified

// We only need one scale and we don't have any chords.
const int minorPentatonicScale[] = {-12, -9, -8, -5, -2, 0, 3, 4, 7, 10, 12};


// We are creating an array to contain all the notes which need to play and
// it will contain a slot for each channel, 0-15, although channel 0x9 (percussion), may
// need to be used in a different way.
// Note, Time on, Time off, Velocity, Expired
// 0000, 1111111, 22222222, 33333333, 4444444
long primaryNotes[16][5][2];
boolean evenOddNotePlaying = false;

// Each time a channel gets prandom values assigned to it, they
// get stored here. The instrument is assigned another way, but
// that will get prandomized another way.
// Velocity is on this list, but that value is set by the 
// potentiometer.
// Pitch offset, Time offset, Play frequency, Duration, Velocity, Instrument
// 000000000000, 11111111111, 22222222222222, 33333333, 44444444, 5555555555
int channelTweaks[16][6];


// Stuff for reading from the SD card
// The array has four sections for the four moods. 
// Each mood has thirty fields for the instruments and behaviors of that mood, but
// not all the fields need to be populated.
const int moodVariables = 50;
int moods[4][moodVariables];

const char instrument_000[] PROGMEM = "Acou Grand Piano";
const char instrument_001[] PROGMEM = " Brt Acou Piano ";
const char instrument_002[] PROGMEM = "Elec Grand Piano";
const char instrument_003[] PROGMEM = "Honky-tonk Piano";
const char instrument_004[] PROGMEM = "Electric Piano 1";
const char instrument_005[] PROGMEM = "Electric Piano 2";
const char instrument_006[] PROGMEM = "  Harpsichord   ";
const char instrument_007[] PROGMEM = "    Clavinet    ";
const char instrument_008[] PROGMEM = "    Celesta     ";
const char instrument_009[] PROGMEM = "  Glockenspiel  ";
const char instrument_010[] PROGMEM = "   Music Box    ";
const char instrument_011[] PROGMEM = "   Vibraphone   ";
const char instrument_012[] PROGMEM = "    Marimba     ";
const char instrument_013[] PROGMEM = "   Xylophone    ";
const char instrument_014[] PROGMEM = " Tubular Bells  ";
const char instrument_015[] PROGMEM = "    Dulcimer    ";
const char instrument_016[] PROGMEM = " Drawbar Organ  ";
const char instrument_017[] PROGMEM = "Percussive Organ";
const char instrument_018[] PROGMEM = "   Rock Organ   ";
const char instrument_019[] PROGMEM = "  Church Organ  ";
const char instrument_020[] PROGMEM = "   Reed Organ   ";
const char instrument_021[] PROGMEM = "   Accordion    ";
const char instrument_022[] PROGMEM = "   Harmonica    ";
const char instrument_023[] PROGMEM = "Tango Accordion ";
const char instrument_024[] PROGMEM = "Ac Guitar(Nylon)";
const char instrument_025[] PROGMEM = "Ac Guitar(Steel)";
const char instrument_026[] PROGMEM = "Elc Guitar(Jazz)";
const char instrument_027[] PROGMEM = "Ec Guitar(Clean)";
const char instrument_028[] PROGMEM = "Ec Guitar(Muted)";
const char instrument_029[] PROGMEM = "Overdrive Guitar";
const char instrument_030[] PROGMEM = "  Dist Guitar   ";
const char instrument_031[] PROGMEM = "Guitar Harmonics";
const char instrument_032[] PROGMEM = " Acoustic Bass  ";
const char instrument_033[] PROGMEM = "Ec Bass (Finger)";
const char instrument_034[] PROGMEM = " Ec Bass (Pick) ";
const char instrument_035[] PROGMEM = " Fretless Bass  ";
const char instrument_036[] PROGMEM = "  Slap Bass 1   ";
const char instrument_037[] PROGMEM = "  Slap Bass 2   ";
const char instrument_038[] PROGMEM = "  Synth Bass 1  ";
const char instrument_039[] PROGMEM = "  Synth Bass 2  ";
const char instrument_040[] PROGMEM = "     Violin     ";
const char instrument_041[] PROGMEM = "     Viola      ";
const char instrument_042[] PROGMEM = "     Cello      ";
const char instrument_043[] PROGMEM = "   Contrabass   ";
const char instrument_044[] PROGMEM = "Tremolo Strings ";
const char instrument_045[] PROGMEM = "Pizzicato Stngs ";
const char instrument_046[] PROGMEM = "Orchestral Harp ";
const char instrument_047[] PROGMEM = "    Timpani     ";
const char instrument_048[] PROGMEM = "String Ensemble1";
const char instrument_049[] PROGMEM = "String Ensemble2";
const char instrument_050[] PROGMEM = "Synth Strings 1 ";
const char instrument_051[] PROGMEM = "Synth Strings 2 ";
const char instrument_052[] PROGMEM = "   Choir Aahs   ";
const char instrument_053[] PROGMEM = "   Voice Oohs   ";
const char instrument_054[] PROGMEM = "  Synth Choir   ";
const char instrument_055[] PROGMEM = " Orchestra Hit  ";
const char instrument_056[] PROGMEM = "    Trumpet     ";
const char instrument_057[] PROGMEM = "    Trombone    ";
const char instrument_058[] PROGMEM = "      Tuba      ";
const char instrument_059[] PROGMEM = " Muted Trumpet  ";
const char instrument_060[] PROGMEM = "  French Horn   ";
const char instrument_061[] PROGMEM = " Brass Section  ";
const char instrument_062[] PROGMEM = " Synth Brass 1  ";
const char instrument_063[] PROGMEM = " Synth Brass 2  ";
const char instrument_064[] PROGMEM = "  Soprano Sax   ";
const char instrument_065[] PROGMEM = "    Alto Sax    ";
const char instrument_066[] PROGMEM = "   Tenor Sax    ";
const char instrument_067[] PROGMEM = "  Baritone Sax  ";
const char instrument_068[] PROGMEM = "      Oboe      ";
const char instrument_069[] PROGMEM = "  English Horn  ";
const char instrument_070[] PROGMEM = "    Bassoon     ";
const char instrument_071[] PROGMEM = "    Clarinet    ";
const char instrument_072[] PROGMEM = "    Piccolo     ";
const char instrument_073[] PROGMEM = "     Flute      ";
const char instrument_074[] PROGMEM = "    Recorder    ";
const char instrument_075[] PROGMEM = "   Pan Flute    ";
const char instrument_076[] PROGMEM = "  Blown bottle  ";
const char instrument_077[] PROGMEM = "   Shakuhachi   ";
const char instrument_078[] PROGMEM = "    Whistle     ";
const char instrument_079[] PROGMEM = "    Ocarina     ";
const char instrument_080[] PROGMEM = "Lead 1 (Square) ";
const char instrument_081[] PROGMEM = "Lead 2(Sawtooth)";
const char instrument_082[] PROGMEM = "Lead 3(Calliope)";
const char instrument_083[] PROGMEM = " Lead 4 (Chiff) ";
const char instrument_084[] PROGMEM = "Lead 5 (Charang)";
const char instrument_085[] PROGMEM = " Lead 6 (Voice) ";
const char instrument_086[] PROGMEM = "Lead 7 (Fifths) ";
const char instrument_087[] PROGMEM = "Lead8(Bass+Lead)";
const char instrument_088[] PROGMEM = "Pad 1 (New Age) ";
const char instrument_089[] PROGMEM = "  Pad 2 (Warm)  ";
const char instrument_090[] PROGMEM = "Pad 3(Polysynth)";
const char instrument_091[] PROGMEM = " Pad 4 (Choir)  ";
const char instrument_092[] PROGMEM = " Pad 5 (Bowed)  ";
const char instrument_093[] PROGMEM = "Pad 6 (Metallic)";
const char instrument_094[] PROGMEM = "  Pad 7 (Halo)  ";
const char instrument_095[] PROGMEM = " Pad 8 (Sweep)  ";
const char instrument_096[] PROGMEM = "  FX 1 (Rain)   ";
const char instrument_097[] PROGMEM = "FX 2(Soundtrack)";
const char instrument_098[] PROGMEM = " FX 3 (Crystal) ";
const char instrument_099[] PROGMEM = "FX 4(Atmosphere)";
const char instrument_100[] PROGMEM = "FX 5(Brightness)";
const char instrument_101[] PROGMEM = " FX 6 (Goblins) ";
const char instrument_102[] PROGMEM = " FX 7 (Echoes)  ";
const char instrument_103[] PROGMEM = " FX 8 (Sci-fi)  ";
const char instrument_104[] PROGMEM = "     Sitar      ";
const char instrument_105[] PROGMEM = "     Banjo      ";
const char instrument_106[] PROGMEM = "    Shamisen    ";
const char instrument_107[] PROGMEM = "      Koto      ";
const char instrument_108[] PROGMEM = "    Kalimba     ";
const char instrument_109[] PROGMEM = "    Bagpipe     ";
const char instrument_110[] PROGMEM = "     Fiddle     ";
const char instrument_111[] PROGMEM = "     Shanai     ";
const char instrument_112[] PROGMEM = "  Tinkle Bell   ";
const char instrument_113[] PROGMEM = "     Agogo      ";
const char instrument_114[] PROGMEM = "  Steel Drums   ";
const char instrument_115[] PROGMEM = "   Woodblock    ";
const char instrument_116[] PROGMEM = "   Taiko Drum   ";
const char instrument_117[] PROGMEM = "  Melodic Tom   ";
const char instrument_118[] PROGMEM = "   Synth Drum   ";
const char instrument_119[] PROGMEM = " Reverse Cymbal ";
const char instrument_120[] PROGMEM = "  Guitar Fret   ";
const char instrument_121[] PROGMEM = "  Breath Noise  ";
const char instrument_122[] PROGMEM = "    Seashore    ";
const char instrument_123[] PROGMEM = "   Bird Tweet   ";
const char instrument_124[] PROGMEM = " Telephone Ring ";
const char instrument_125[] PROGMEM = "   Helicopter   ";
const char instrument_126[] PROGMEM = "    Applause    ";
const char instrument_127[] PROGMEM = "    Gunshot     ";

const char *const MIDIList[128] PROGMEM = {
  instrument_000,
  instrument_001,
  instrument_002,
  instrument_003,
  instrument_004,
  instrument_005,
  instrument_006,
  instrument_007,
  instrument_008,
  instrument_009,
  instrument_010,
  instrument_011,
  instrument_012,
  instrument_013,
  instrument_014,
  instrument_015,
  instrument_016,
  instrument_017,
  instrument_018,
  instrument_019,
  instrument_020,
  instrument_021,
  instrument_022,
  instrument_023,
  instrument_024,
  instrument_025,
  instrument_026,
  instrument_027,
  instrument_028,
  instrument_029,
  instrument_030,
  instrument_031,
  instrument_032,
  instrument_033,
  instrument_034,
  instrument_035,
  instrument_036,
  instrument_037,
  instrument_038,
  instrument_039,
  instrument_040,
  instrument_041,
  instrument_042,
  instrument_043,
  instrument_044,
  instrument_045,
  instrument_046,
  instrument_047,
  instrument_048,
  instrument_049,
  instrument_050,
  instrument_051,
  instrument_052,
  instrument_053,
  instrument_054,
  instrument_055,
  instrument_056,
  instrument_057,
  instrument_058,
  instrument_059,
  instrument_060,
  instrument_061,
  instrument_062,
  instrument_063,
  instrument_064,
  instrument_065,
  instrument_066,
  instrument_067,
  instrument_068,
  instrument_069,
  instrument_070,
  instrument_071,
  instrument_072,
  instrument_073,
  instrument_074,
  instrument_075,
  instrument_076,
  instrument_077,
  instrument_078,
  instrument_079,
  instrument_080,
  instrument_081,
  instrument_082,
  instrument_083,
  instrument_084,
  instrument_085,
  instrument_086,
  instrument_087,
  instrument_088,
  instrument_089,
  instrument_090,
  instrument_091,
  instrument_092,
  instrument_093,
  instrument_094,
  instrument_095,
  instrument_096,
  instrument_097,
  instrument_098,
  instrument_099,
  instrument_100,
  instrument_101,
  instrument_102,
  instrument_103,
  instrument_104,
  instrument_105,
  instrument_106,
  instrument_107,
  instrument_108,
  instrument_109,
  instrument_110,
  instrument_111,
  instrument_112,
  instrument_113,
  instrument_114,
  instrument_115,
  instrument_116,
  instrument_117,
  instrument_118,
  instrument_119,
  instrument_120,
  instrument_121,
  instrument_122,
  instrument_123,
  instrument_124,
  instrument_125,
  instrument_126,
  instrument_127
};
char buffer[16];
char moodName[4][32];

bool verboseFeedback = false;
bool showSliders = false;
int incomingByte = 0;

/////////////////////////////////////////////////
/////////2/4///H/O/U/R///E/N/G/I/N/E/E/R/////////
/////////////////////////////////////////////////

void setup(){
  // Setup the LCD
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
  splashScreen();
  
  Serial.begin(9600);
  Serial2.begin(31250); // MIDI uses a 'strange baud rate'
  
  pinMode(onboardLED, OUTPUT);
  digitalWrite(onboardLED, LOW);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(verticalPotentiometer, INPUT);
  pinMode(horizontalPotentiometer, INPUT);
  
  midiSetInstrument(0, 4);
  channelTweaks[0][0] = 0;
  channelTweaks[0][1] = 0;
  channelTweaks[0][2] = 100;
  channelTweaks[0][3] = 10;
  channelTweaks[0][4] = mainVelocity;
  
  pinMode(53, OUTPUT);
  if (!SD.begin(53)) {
    Serial.print(F("SD card initialization failed!"));
    Serial.print("\n");
    cardNotFound();
    while (1);
  }
  SDCardStart();
  clearScreen();
}

void loop() {
  // screen variables
  int buf[478];
  int x, x2;
  int y, y2;
  int r;
  
  checkDiscreteInputs();
  checkAnalogInputs();
  remainCalm();
  if(analogChange){
    updateAttentionPercentages();
  }
  if (buttonStates[panicButton]){
    timeThisCycle = millis();
    actOnMode01();
    maintainMoods();
  }
  readSerial();
}
