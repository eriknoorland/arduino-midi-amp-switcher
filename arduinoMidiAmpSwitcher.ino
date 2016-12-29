#include "Adafruit_LiquidCrystal.h"
#include "EEPROM.h"
#include "Wire.h"
#include "MIDI.h"

// software version number (major.minor.hotfix)
const String VERSION_NUMBER = "0.7.0";

// default midi channel is used when the user
// has not yet stored a midi channel
const byte DEFAULT_MIDI_CHANNEL = 2;

// input pins (rotary / buttons)
const byte ROTARY_PIN_A = 2;
const byte ROTARY_PIN_B = 3;
const byte SELECT_BUTTON_PIN = 4;
const byte STORE_BUTTON_PIN = 11;
const byte EXIT_BUTTON_PIN = 12;

// output pins
const byte ACTIVITY_PIN = 13;
const byte OUTPUT_PIN_1 = 6;
const byte OUTPUT_PIN_2 = 7;
const byte OUTPUT_PIN_3 = 8;
const byte OUTPUT_PIN_4 = 9;

// program states
const byte STATE_INIT = 0;
const byte STATE_NAV = 1;
const byte STATE_SELECT_MIDI_CHANNEL = 2;
const byte STATE_SELECT_MIDI_PROGRAM_PRESET_NUMBER = 3;
const byte STATE_SELECT_MIDI_PROGRAM_NUMBER = 4;
const byte STATE_SELECT_MIDI_PROGRAM_PRESET = 5;
const byte STATE_SELECT_MIDI_CONTROL_CHANGE_OUTPUT = 6;
const byte STATE_SELECT_MIDI_CONTROL_CHANGE_NUMBER = 7;
const byte STATE_MEMORY_RESET = 8;
const byte STATE_SOFTWARE_VERSION = 9;

// rotary value maximums
const byte NUM_NAVIGATION_ITEMS = 5;
const byte NUM_DEVICE_OUTPUTS = 4;
const byte NUM_MIDI_CHANNELS = 128;
const byte NUM_PROGRAM_NUMBERS = 128;
const byte NUM_CC_NUMBERS = 128;
const byte NUM_PROGRAM_PRESETS = 16;

// a translation table from nav items to program states
const byte NAVIGATION_ITEMS_TO_STATE[NUM_NAVIGATION_ITEMS] = {
  STATE_SELECT_MIDI_CHANNEL,
  STATE_SELECT_MIDI_PROGRAM_PRESET_NUMBER,
  STATE_SELECT_MIDI_CONTROL_CHANGE_OUTPUT,
  STATE_MEMORY_RESET,
  STATE_SOFTWARE_VERSION
};

// navigation items
const String NAVIGATION_ITEMS[NUM_NAVIGATION_ITEMS] = {"CHANNEL", "PROGRAM", "CC", "RESET MEMORY", "VERSION"};

// program presets
const String PROGRAM_PRESET_STRINGS[NUM_PROGRAM_PRESETS] = {
  "0000", "1000", "0100", "0010", "0001", "1001", "1010", "1011",
  "1100", "1101", "1110", "1111", "0011", "0101", "0110", "0111"
};

// device outputs
const byte DEVICE_OUTPUTS[NUM_DEVICE_OUTPUTS] = {1, 2, 3, 4};
const byte DEVICE_OUTPUT_PINS[NUM_DEVICE_OUTPUTS] = {OUTPUT_PIN_1, OUTPUT_PIN_2, OUTPUT_PIN_3, OUTPUT_PIN_4};

// storage locations
const int MIDI_CHANNEL_STORAGE_LOCATION = 37;
const int MIDI_CC_STORAGE_LOCATIONS[NUM_DEVICE_OUTPUTS] = {33, 34, 35, 36};

// rotary variables
volatile byte rotaryPinAFlag = 0;
volatile byte rotaryPinBFlag = 0;
volatile int newRotaryValue = 0;
volatile int prevRotaryValue = 0;
volatile byte pinReading = 0;

boolean selectButtonState = LOW;
boolean selectButtonLastState = LOW;
boolean storeButtonState = LOW;
boolean storeButtonLastState = LOW;
boolean exitButtonState = LOW;
boolean exitButtonLastState = LOW;

int rotaryValue;
byte minRotaryValue;
byte maxRotaryValue;
boolean loopRotaryValue = true;

byte state = -1;

byte tempMidiChannel;
byte tempProgramPresetNumber;
byte tempProgramNumber;
byte tempProgramPreset;
byte tempOutputNumber;
byte tempCCNumber;

Adafruit_LiquidCrystal lcd(0);

MIDI_CREATE_DEFAULT_INSTANCE();

/////////////////////////
////  RESET HELPERS  ////
/////////////////////////

/**
 * Resets the rotary value
 * @param {byte} value [optional] defaults to 0
 */
void resetRotaryValue(byte value = 0) {
  newRotaryValue = value;
  prevRotaryValue = value;
  rotaryValue = value;
  rotaryPinAFlag = 0;
  rotaryPinBFlag = 0;
  pinReading = 0;
}

/**
 * Resets the temporary values
 */
void resetTempValues() {
  tempMidiChannel = 0;
  tempProgramPresetNumber = 0;
  tempProgramNumber = 0;
  tempProgramPreset = 0;
  tempOutputNumber = 0;
  tempCCNumber = 0;
}

/**
 * Flashes the activity LED
 */
void onMidiActivity() {
  digitalWrite(ACTIVITY_PIN, HIGH);
  delay(40);
  digitalWrite(ACTIVITY_PIN, LOW);
  delay(40);
  digitalWrite(ACTIVITY_PIN, HIGH);
  delay(40);
  digitalWrite(ACTIVITY_PIN, LOW);
}

//////////////////////////
////  EEPROM HELPERS  ////
//////////////////////////

/**
 * Returns the stored value at the given location or 0
 * @param {byte} location
 * @return {byte}
 */
byte getValueFromStorage(int location) {
  byte value = EEPROM.read(location);

  if(value == 255) {
    value = 0;
  }

  return value;
}

/**
 * Stores the value in the given location when it's an altered value
 * @param {byte} location
 * @param {byte} value
 */
void storeValue(byte location, byte value) {
  byte currentValue = EEPROM.read(location);

  if(currentValue != value) {
    // EEPROM.write(location, value);
  }
}

///////////////////////////
////  DISPLAY HELPERS  ////
///////////////////////////

/**
 * Updates the given line on the LCD display
 * @param {string} body [optional]
 * @param {byte} lineNumber [optional]
 */
void updateDisplayLine(String body = "", byte lineNumber = 0) {
  if(body != "") {
    lcd.setCursor(0, lineNumber);
    lcd.print("                ");
    lcd.setCursor(0, lineNumber);
    lcd.print(body);
  }
}

/**
 * Updates the LCD display with the given copy
 * @param {String} line1 [optional]
 * @param {String} line2 [optional]
 * @param {Boolean} clear [optional]
 */
void updateDisplay(String line1 = "", String line2 = "", boolean clear = false) {
  if(clear) {
    lcd.clear();
  }

  updateDisplayLine(line1, 0);
  updateDisplayLine(line2, 1);
}

/**
 * Updates the LCD display based on the rotary values
 * when settings the various parameters
 * @param {String} info
 * @param {int} numPreset
 * @param {int} numProgram
 * @param {String} preset
 */
void updateMidiProgramDisplay(String info, int numPreset, int numProgram, byte preset) {
  String line1 = NAVIGATION_ITEMS[1] + " " + info;
  String line2 = "P" + String(numPreset) + " / " + String(numProgram) + " / " + PROGRAM_PRESET_STRINGS[preset];

  updateDisplay(line1, line2, true);
}

/**
 * Updates the LCD display based on the rotary values
 * when settings the various parameters
 * @param {String} info
 * @param {int} numOutput
 * @param {int} numCC
 */
void updateMidiCCDisplay(String info, int numOutput, int numCC) {
  String line1 = NAVIGATION_ITEMS[2] + " " + info;
  String line2 = String(DEVICE_OUTPUTS[numOutput]) + " / " + String(numCC);

  updateDisplay(line1, line2, true);
}

//////////////////////////
////  INPUT HANDLERS  ////
//////////////////////////

/**
 * Handler for when switching to the init state
 */
void onInitState() {
  updateDisplay("CUSTOM MIDI", "AMP SWITCHER", true);
}

/**
 * Handler for when switching to the navigation state
 */
void onNavigationState() {
  onNavigationChange(0);
}

/**
 * Updates the LCD display based on the rotary value
 * when scrolling through the navigation menu
 */
void onNavigationChange(byte index) {
  int secondNavItem = index + 1;

  String line1 = NAVIGATION_ITEMS[index] + " <";
  String line2;

  if(secondNavItem < NUM_NAVIGATION_ITEMS) {
    line2 = NAVIGATION_ITEMS[secondNavItem];
  }
  
  updateDisplay(line1, line2, true);
}

/**
 * Handler for when switching to the memory reset state
 */
void onMemoryResetState() {
  updateDisplay(NAVIGATION_ITEMS[3], "store or exit", true);
}

/**
 * Resets the memory
 */
void onStoreMemoryReset() {
  for(int i = 0; i < EEPROM.length(); i++) {
    storeValue(i, 0);
  }
}

/**
 * Handler for when switching to the software version state
 */
void onSoftwareVersionState() {
  updateDisplay(NAVIGATION_ITEMS[4], VERSION_NUMBER, true);
}

/////////////////////////////////
////  MIDI CHANNEL HANDLERS  ////
/////////////////////////////////

/**
 * Handler for when switching to the midi channel state
 */
void onMidiChannelState() {
  byte channel = getValueFromStorage(MIDI_CHANNEL_STORAGE_LOCATION);

  resetRotaryValue(channel);
  onMidiChannelChange(channel);
}

/**
 * Handler for when the midi channel value changes
 * @param {byte} value
 */
void onMidiChannelChange(byte value) {
  tempMidiChannel = value;

  updateDisplay(NAVIGATION_ITEMS[0], String(tempMidiChannel));
}

/**
 * Stores the midi channel
 */
void onStoreMidiChannel() {
  storeValue(MIDI_CHANNEL_STORAGE_LOCATION, tempMidiChannel);
}

/////////////////////////////////
////  MIDI PROGRAM HANDLERS  ////
/////////////////////////////////

/**
 * Handler for when switching to the midi program preset number state
 */
void onMidiProgramPresetNumberState() {
  resetRotaryValue(0);
  onMidiProgramPresetNumberChange(0);
}

/**
 * Handler for when the midi program preset number value changes
 * @param {byte} value
 */
void onMidiProgramPresetNumberChange(byte value) {
  tempProgramPresetNumber = value + 1;
  tempProgramNumber = getValueFromStorage(tempProgramPresetNumber);
  tempProgramPreset = getValueFromStorage(NUM_PROGRAM_PRESETS + tempProgramPresetNumber);
  updateMidiProgramDisplay("id", tempProgramPresetNumber, tempProgramNumber, tempProgramPreset);
}

/**
 * Handler for when switching to the midi program number state
 */
void onMidiProgramNumberState() {
  resetRotaryValue(tempProgramNumber);
  onMidiProgramNumberChange(tempProgramNumber);
}

/**
 * Handler for when the midi program number value changes
 * @param {byte} value
 */
void onMidiProgramNumberChange(byte value) {
  tempProgramNumber = value;
  updateMidiProgramDisplay("program", tempProgramPresetNumber, tempProgramNumber, tempProgramPreset);
}

/**
 * Handler for when switching to the midi program preset state
 */
void onMidiProgramPresetState() {
  resetRotaryValue(tempProgramPreset);
  onMidiProgramNumberChange(tempProgramPreset);
}

/**
 * Handler for when the midi program preset value changes
 * @param {byte} value
 */
void onMidiProgramPresetChange(byte value) {
  tempProgramPreset = value;
  updateMidiProgramDisplay("preset", tempProgramPresetNumber, tempProgramNumber, tempProgramPreset);
}

/**
 * Stores the midi program preset code
 */
void onStoreMidiProgramPreset() {
  storeValue(tempProgramPresetNumber, tempProgramNumber);
  storeValue(NUM_PROGRAM_PRESETS + tempProgramPresetNumber, tempProgramPreset);
}

////////////////////////////
////  MIDI CC HANDLERS  ////
////////////////////////////

/**
 * Handler for when switching to the midi cc output state
 */
void onMidiCCOutputState() {
  resetRotaryValue(0);
  onMidiCCOutputChange(0);
}

/**
 * Handler for when the midi cc output value changes
 * @param {byte} value
 */
void onMidiCCOutputChange(byte value) {
  tempOutputNumber = value;
  tempCCNumber = getValueFromStorage(MIDI_CC_STORAGE_LOCATIONS[tempOutputNumber]);
  updateMidiCCDisplay("output", tempOutputNumber, tempCCNumber);
}

/**
 * Handler for when switching to the midi cc number state
 */
void onMidiCCNumberState() {
  resetRotaryValue(tempCCNumber);
  onMidiCCChange(tempCCNumber);
}

/**
 * Handler for when the midi cc value changes
 * @param {byte} value
 */
void onMidiCCChange(byte value) {
  tempCCNumber = value;
  updateMidiCCDisplay("ctl", tempOutputNumber, tempCCNumber);
}

/**
 * Stores the midi cc code
 */
void onStoreMidiCC() {
  storeValue(MIDI_CC_STORAGE_LOCATIONS[tempOutputNumber], tempCCNumber);
}

///////////////////////////////
////  MIDI INPUT HANDLERS  ////
///////////////////////////////

/**
 * Control change handler
 * @param {byte} channel
 * @param {byte} number
 * @param {byte} value
 */
void onControlChange(byte channel, byte number, byte value) {
  byte cc;
  byte pin;

  for(byte i = 0; i < NUM_DEVICE_OUTPUTS; i++) {
    cc = getValueFromStorage(MIDI_CC_STORAGE_LOCATIONS[i]);
    pin = DEVICE_OUTPUT_PINS[i];

    if(cc == number) {
      digitalWrite(pin, (value ? HIGH : LOW));
    }
  }

  onMidiActivity();
}

/**
 * Program change handler
 * @param {byte} channel
 * @param {byte} number
 */
void onProgramChange(byte channel, byte number) {
  String preset;
  byte program;
  byte pinValue;
  byte pin;

  for(byte i = 1; i < NUM_PROGRAM_PRESETS + 1; i++) {
    program = getValueFromStorage(j);

    if(program == number) {
      preset = getValueFromStorage(NUM_PROGRAM_PRESETS + j);

      for(byte j = 0; j < 4; j++) {
        pin = DEVICE_OUTPUT_PINS[j];
        pinValue = (String(preset[j]) == "1" ? HIGH : LOW);
        digitalWrite(pin, pinValue);
      }
    }
  }

  onMidiActivity();
}

////////////////////////////
////  INPUT COLLECTORS  ////
////////////////////////////

/**
 * Rotary pin A change handler
 */
void onRotaryUpdatePinA() {
  cli();
  pinReading = PIND & 0xC;

  if(pinReading == B00001100 && rotaryPinAFlag) {
    newRotaryValue--;
    rotaryPinBFlag = 0;
    rotaryPinAFlag = 0;
  } else if(pinReading == B00000100) {
    rotaryPinBFlag = 1;
  }

  sei();
}

/**
 * Rotary pin B change handler
 */
void onRotaryUpdatePinB() {
  cli();
  pinReading = PIND & 0xC;

  if(pinReading == B00001100 && rotaryPinBFlag) {
    newRotaryValue++;
    rotaryPinBFlag = 0;
    rotaryPinAFlag = 0;
  } else if(pinReading == B00001000) {
    rotaryPinAFlag = 1;
  }

  sei();
}

/**
 * Rotary value handler
 */
void onRotaryUpdate() {
  // no rotation means no continue
  if(prevRotaryValue == newRotaryValue) {
    return;
  }

  prevRotaryValue = newRotaryValue;

  // when hitting the maximum value either stop
  // or continue from the minimum value
  if(newRotaryValue >= maxRotaryValue) {
    newRotaryValue = loopRotaryValue ? minRotaryValue : maxRotaryValue - 1;
  }

  // when hitting the minimum value either stop
  // or continue from the maximum value
  if(newRotaryValue <= minRotaryValue - 1) {
    newRotaryValue = loopRotaryValue ? maxRotaryValue - 1 : minRotaryValue;
  }

  // no value change, no need for updating
  if(newRotaryValue == rotaryValue) {
    return;
  }

  rotaryValue = newRotaryValue;

  switch(state) {
    case STATE_NAV:
      onNavigationChange(rotaryValue);
      break;

    case STATE_SELECT_MIDI_CHANNEL:
      onMidiChannelChange(rotaryValue);
      break;

    case STATE_SELECT_MIDI_PROGRAM_PRESET_NUMBER:
      onMidiProgramPresetNumberChange(rotaryValue);
      break;

    case STATE_SELECT_MIDI_PROGRAM_NUMBER:
      onMidiProgramNumberChange(rotaryValue);
      break;

    case STATE_SELECT_MIDI_PROGRAM_PRESET:
      onMidiProgramPresetChange(rotaryValue);
      break;

    case STATE_SELECT_MIDI_CONTROL_CHANGE_OUTPUT:
      onMidiCCOutputChange(rotaryValue);
      break;

    case STATE_SELECT_MIDI_CONTROL_CHANGE_NUMBER:
      onMidiCCChange(rotaryValue);
      break;
  }
}

/**
 * Select button press handler
 */
void onSelectButtonPress() {
  switch(state) {
    case STATE_INIT:
      changeState(STATE_NAV);
      break;

    case STATE_NAV:
      changeState(NAVIGATION_ITEMS_TO_STATE[rotaryValue]);
      break;

    case STATE_SELECT_MIDI_PROGRAM_PRESET_NUMBER:
      changeState(STATE_SELECT_MIDI_PROGRAM_NUMBER);
      break;

    case STATE_SELECT_MIDI_PROGRAM_NUMBER:
      changeState(STATE_SELECT_MIDI_PROGRAM_PRESET);
      break;

    case STATE_SELECT_MIDI_CONTROL_CHANGE_OUTPUT:
      changeState(STATE_SELECT_MIDI_CONTROL_CHANGE_NUMBER);
      break;
  }
}

/**
 * Store button press handler
 */
void onStoreButtonPress() {
  switch(state) {
    case STATE_SELECT_MIDI_CHANNEL:
      onStoreMidiChannel();
      changeState(STATE_NAV);
      break;

    case STATE_SELECT_MIDI_PROGRAM_PRESET:
      onStoreMidiProgramPreset();
      changeState(STATE_NAV);
      break;

    case STATE_SELECT_MIDI_CONTROL_CHANGE_NUMBER:
      onStoreMidiCC();
      changeState(STATE_NAV);
      break;

    case STATE_MEMORY_RESET:
      onStoreMemoryReset();
      changeState(STATE_NAV);
      break;
  }

  resetTempValues();
}

/**
 * Exit button press handler
 */
void onExitButtonPress() {
  byte tempState;
  
  if(state != STATE_INIT) {
    tempState = STATE_NAV;

    if(state == STATE_NAV) {
      tempState = STATE_INIT;
    }

    resetTempValues();
    resetRotaryValue();
    changeState(tempState);
  }
}

/**
 * Changes the state of the application
 * @param {int} newState
 */
void changeState(int newState) {
  if(state == newState) {
    return;
  }
  
  state = newState;

  switch(newState) {
    case STATE_INIT:
      onInitState();
      break;

    case STATE_NAV:
      minRotaryValue = 0;
      maxRotaryValue = NUM_NAVIGATION_ITEMS;
      loopRotaryValue = false;
      onNavigationState();
      resetRotaryValue(minRotaryValue);
      break;

    case STATE_SELECT_MIDI_CHANNEL:
      minRotaryValue = 1;
      maxRotaryValue = NUM_MIDI_CHANNELS + 1;
      loopRotaryValue = true;
      onMidiChannelState();
      break;

    case STATE_SELECT_MIDI_PROGRAM_PRESET_NUMBER:
      minRotaryValue = 0;
      maxRotaryValue = NUM_PROGRAM_PRESETS;
      loopRotaryValue = true;
      onMidiProgramPresetNumberState();
      break;

    case STATE_SELECT_MIDI_PROGRAM_NUMBER:
      minRotaryValue = 1;
      maxRotaryValue = NUM_PROGRAM_NUMBERS + 1;
      loopRotaryValue = true;
      onMidiProgramNumberState();
      break;

    case STATE_SELECT_MIDI_PROGRAM_PRESET:
      minRotaryValue = 0;
      maxRotaryValue = NUM_PROGRAM_PRESETS;
      loopRotaryValue = true;
      onMidiProgramPresetState();
      break;

    case STATE_SELECT_MIDI_CONTROL_CHANGE_OUTPUT:
      minRotaryValue = 0;
      maxRotaryValue = NUM_DEVICE_OUTPUTS;
      loopRotaryValue = true;
      onMidiCCOutputState();
      break;

    case STATE_SELECT_MIDI_CONTROL_CHANGE_NUMBER:
      minRotaryValue = 1;
      maxRotaryValue = NUM_CC_NUMBERS + 1;
      loopRotaryValue = true;
      onMidiCCNumberState();
      break;

    case STATE_MEMORY_RESET:
      onMemoryResetState();
      break;

    case STATE_SOFTWARE_VERSION:
      onSoftwareVersionState();
      break;
  }
}

/**
 * Setup
 */
void setup() {
  lcd.setBacklight(HIGH);
  lcd.begin(16, 2);
  
  pinMode(ROTARY_PIN_A, INPUT);
  pinMode(ROTARY_PIN_B, INPUT);
  digitalWrite(ROTARY_PIN_A, HIGH);
  digitalWrite(ROTARY_PIN_B, HIGH);
  attachInterrupt(0, onRotaryUpdatePinA, RISING);
  attachInterrupt(1, onRotaryUpdatePinB, RISING);

  digitalWrite(SELECT_BUTTON_PIN, HIGH);
  selectButtonState = digitalRead(SELECT_BUTTON_PIN);
  selectButtonLastState = selectButtonState;

  digitalWrite(STORE_BUTTON_PIN, HIGH);
  storeButtonState = digitalRead(STORE_BUTTON_PIN);
  storeButtonLastState = storeButtonState;

  digitalWrite(EXIT_BUTTON_PIN, HIGH);
  exitButtonState = digitalRead(EXIT_BUTTON_PIN);
  exitButtonLastState = exitButtonState;

  pinMode(ACTIVITY_PIN, OUTPUT);
  pinMode(OUTPUT_PIN_1, OUTPUT);
  pinMode(OUTPUT_PIN_2, OUTPUT);
  pinMode(OUTPUT_PIN_3, OUTPUT);
  pinMode(OUTPUT_PIN_4, OUTPUT);

  byte midiChannel = getValueFromStorage(MIDI_CHANNEL_STORAGE_LOCATION);

  if(midiChannel == 0) {
    midiChannel = DEFAULT_MIDI_CHANNEL;
  }
  
  MIDI.begin(midiChannel);
  MIDI.setHandleControlChange(onControlChange);
  MIDI.setHandleProgramChange(onProgramChange);

  resetTempValues();
  changeState(STATE_INIT);
}

/**
 * Loop
 */
void loop() {
  MIDI.read();
  
  selectButtonState = digitalRead(SELECT_BUTTON_PIN);
  
  if(selectButtonState == HIGH && selectButtonLastState == LOW) {
   onSelectButtonPress();
  }
  delay(1);
  
  selectButtonLastState = selectButtonState;

  // no need to check button and rotary states when in init mode
  if(state != STATE_INIT) {
    storeButtonState = digitalRead(STORE_BUTTON_PIN);
    exitButtonState = digitalRead(EXIT_BUTTON_PIN);
  
    if(storeButtonState == LOW && storeButtonLastState == HIGH) {
     onStoreButtonPress();
    }
    delay(1);
  
    if(exitButtonState == LOW && exitButtonLastState == HIGH) {
     onExitButtonPress();
    }
    delay(1);
  
    storeButtonLastState = storeButtonState;
    exitButtonLastState = exitButtonState;
  
    onRotaryUpdate();
  }
}
