#include <MIDI.h>

// software version number (major.minor.hotfix)
const String VERSION_NUMBER = "0.2.0";

// default midi channel is used when the user
// has not yet stored a midi channel
const byte DEFAULT_MIDI_CHANNEL = 1;

// The most left digit stands for the first output
// and the most right digit for the last output
const byte numProgramPresets = 17;
const String programPresets[numProgramPresets] = {
  "----", "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111",
  "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"
};

// output pins
const byte activityPin = 13;
const byte outputPin1 = 9;
const byte outputPin2 = 10;
const byte outputPin3 = 11;
const byte outputPin4 = 12;

const byte numOutputPins = 5;
const byte outputPins[numOutputPins] = {
  activityPin, outputPin1, outputPin2, outputPin3, outputPin4
};

// input pins
const byte rotaryPinA = 2;
const byte rotaryPinB = 3;
const byte selectButtonPin = 4;
const byte storeButtonPin = 5;
const byte exitButtonPin = 6;

boolean selectButtonState = LOW;
boolean selectButtonLastState = LOW;
boolean storeButtonState = LOW;
boolean storeButtonLastState = LOW;
boolean exitButtonState = LOW;
boolean exitButtonLastState = LOW;

const byte numInputPins = 5;
const byte inputPins[numInputPins] = {
  rotaryPinA,
  rotaryPinB,
  selectButtonPin,
  storeButtonPin,
  exitButtonPin
};

const int rotaryBump[] = {0, -1, 0, 1};
byte rotaryValue = 0;
byte minRotaryValue = 0;
byte maxRotaryValue = 10; // number of array length of the current scrolling list
boolean loopRotaryValue = true; // get to the beginning of the array when at the end and vice versa

byte midiChannel = DEFAULT_MIDI_CHANNEL;

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
  // setup inputs using the internal pullup resistors
  for(int j = 0; j < numInputPins; j++) {
    pinMode(inputPins[j], INPUT);
    digitalWrite(inputPins[j], HIGH);
  }

  // setup button states
  selectButtonState = digitalRead(selectButtonPin);
  selectButtonLastState = selectButtonState;
  storeButtonState = digitalRead(storeButtonPin);
  storeButtonLastState = storeButtonState;
  exitButtonState = digitalRead(exitButtonPin);
  exitButtonLastState = exitButtonState;

  // get the user stored channel or use the default
  // midiChannel = <EEPROM data>;

  // setup midi listeners
  MIDI.begin(midiChannel);
  MIDI.setHandleControlChange(onControlChange);
  MIDI.setHandleProgramChange(onProgramChange);

  // setup rotary listener
  attachInterrupt(0, onRotaryUpdate, RISING);

  Serial.begin(9600);
}

/**
 * Program loop
 */
void loop() {
  MIDI.read();

  selectButtonState = digitalRead(selectButtonPin);
  storeButtonState = digitalRead(storeButtonPin);
  exitButtonState = digitalRead(exitButtonPin);

  if(selectButtonState == HIGH && selectButtonLastState == LOW) {
   onSelectButtonPress();
  }
  delay(1);

  if(storeButtonState == LOW && storeButtonLastState == HIGH) {
   onStoreButtonPress();
  }
  delay(1);

  if(exitButtonState == LOW && exitButtonLastState == HIGH) {
   onExitButtonPress();
  }
  delay(1);

  selectButtonLastState = selectButtonState;
  storeButtonLastState = storeButtonState;
  exitButtonLastState = exitButtonState;
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

/**
 * Rotary change handler
 */
void onRotaryUpdate() {
  byte state = 0;

  state += digitalRead(rotaryPinA);
  state <<= 1;
  state += digitalRead(rotaryPinB);

  rotaryValue += rotaryBump[state];

  if(rotaryValue == maxRotaryValue) {
    rotaryValue = loop ? 0 : maxRotaryValue - 1;
  }

  if(rotaryValue < minRotaryValue) {
    rotaryValue = loop ? maxRotaryValue - 1 : 0;
  }

  Serial.println(rotaryValue);
}

/**
 * Select button press handler
 */
void onSelectButtonPress() {
  Serial.println("Select button pressed!");
}

/**
 * Store button press handler
 */
void onStoreButtonPress() {
  Serial.println("Store button pressed!");
}

/**
 * Exit button press handler
 */
void onExitButtonPress() {
  Serial.println("Exit button pressed!");
}
