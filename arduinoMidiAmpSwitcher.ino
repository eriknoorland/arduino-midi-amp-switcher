#include <MIDI.h>

// software version number (major.minor.hotfix)
const String VERSION_NUMBER = "0.1.0";

// default midi channel is used when the user
// has not yet stored a midi channel
const int DEFAULT_MIDI_CHANNEL = 1;

// The most left digit stands for the first output
// and the most right digit for the last output
const int numProgramPresets = 17;
const String programPresets[17] = {
  "----", "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111",
  "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"
};

// output pins
const byte activityPin = 13;
const byte outputPin1 = 13;
const byte outputPin2 = 13;
const byte outputPin3 = 13;
const byte outputPin4 = 13;

int numOutputPins = 5;
int outputPins[5] = {
  activityPin, outputPin1, outputPin2, outputPin3, outputPin4
};

// input pins
const byte rotaryPinA = 3;
const byte rotaryPinB = 3;
const byte selectButtonPin = 3;
const byte storeButtonPin = 3;
const byte exitButtonPin = 3;

int numInputPins = 3;
int inputPins[3] = {
  rotaryPinA,
  rotaryPinB,
  selectButtonPin,
  storeButtonPin,
  exitButtonPin
};

int midiChannel = DEFAULT_MIDI_CHANNEL;

MIDI_CREATE_DEFAULT_INSTANCE();

/**
 * Program setup
 */
void setup() {
  // setup output pin modes
  for(int i = 0; i < numOutputPins; i++) {
    pinMode(outputPins[i], OUTPUT);
  }

  // setup input pin modes
  for(int j = 0; j < numInputPins; j++) {
    pinMode(inputPins[j], INPUT);
  }

  // get the user stored channel or use the default
  // midiChannel = <EEPROM data>;

  // setup midi listeners
  MIDI.begin(midiChannel);
  MIDI.setHandleControlChange(onControlChange);
  MIDI.setHandleProgramChange(onProgramChange);
}

/**
 * Program loop
 */
void loop() {
  MIDI.read();
}

/**
 * Flashes the activity LED
 */
void onMidiActivity() {
  // FIX ME: make this function non-blocking
  digitalWrite(activityPin, HIGH);
  delay(50);
  digitalWrite(activityPin, LOW);
  delay(50);
  digitalWrite(activityPin, HIGH);
  delay(50);
  digitalWrite(activityPin, LOW);
}

/**
 * Control change handler
 * @param {byte} channel
 * @param {byte} number
 * @param {byte} value
 */
void onControlChange(byte channel, byte number, byte value) {
  Serial.println("cc number -> " + String(number));

  // if this number is stored by the user
  // get the output number (1-4)
  // set the output to either HIGH or LOW based on the value

  onMidiActivity();
}

/**
 * Program change handler
 * @param {byte} channel
 * @param {byte} number
 */
void onProgramChange(byte channel, byte number) {
  Serial.println("program number -> " + String(number));

  // if this number is store by the user
  // the stored value will look something like 23_5 <program number>_<programPresets index>
  // get the corresponding preset (---- means it's turned off)
  // loop through the preset and set the outputs accordingly

  onMidiActivity();
}
