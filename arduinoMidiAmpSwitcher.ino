#include "MIDI.h"

#define MIDI_CHANNEL 2
#define ACTIVITY_PIN 13
#define OUTPUT_PIN_1 6
#define OUTPUT_PIN_2 7
#define OUTPUT_PIN_3 8
#define OUTPUT_PIN_4 9

// program presets
const String PROGRAM_PRESET_STRINGS[NUM_PROGRAM_PRESETS] = {
  "0000", "1000", "0100", "0010", "0001", "1001", "1010", "1011",
  "1100", "1101", "1110", "1111", "0011", "0101", "0110", "0111"
};

// device outputs
// const byte DEVICE_OUTPUTS[NUM_DEVICE_OUTPUTS] = {1, 2, 3, 4};
// const byte DEVICE_OUTPUT_PINS[NUM_DEVICE_OUTPUTS] = {OUTPUT_PIN_1, OUTPUT_PIN_2, OUTPUT_PIN_3, OUTPUT_PIN_4};

// storage locations
// const int MIDI_CHANNEL_STORAGE_LOCATION = 37;
// const int MIDI_CC_STORAGE_LOCATIONS[NUM_DEVICE_OUTPUTS] = {33, 34, 35, 36};

MIDI_CREATE_DEFAULT_INSTANCE();

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

/**
 * Control change handler
 * @param {byte} channel
 * @param {byte} number
 * @param {byte} value
 */
void onControlChange(byte channel, byte number, byte value) {
  if (channel == MIDI_CHANNEL) {
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
}

/**
 * Program change handler
 * @param {byte} channel
 * @param {byte} number
 */
void onProgramChange(byte channel, byte number) {
  if (channel == MIDI_CHANNEL) {
    String preset;
    byte program;
    byte pinValue;
    byte pin;

    for(byte i = 1; i < NUM_PROGRAM_PRESETS + 1; i++) {
      program = getValueFromStorage(i);

      if(program == number) {
        preset = getValueFromStorage(NUM_PROGRAM_PRESETS + i);

        for(byte j = 0; j < 4; j++) {
          pin = DEVICE_OUTPUT_PINS[j];
          pinValue = (String(preset[j]) == "1" ? HIGH : LOW);
          digitalWrite(pin, pinValue);
        }
      }
    }

    onMidiActivity();
  }
}

/**
 * Setup
 */
void setup() {
  pinMode(ACTIVITY_PIN, OUTPUT);
  pinMode(OUTPUT_PIN_1, OUTPUT);
  pinMode(OUTPUT_PIN_2, OUTPUT);
  pinMode(OUTPUT_PIN_3, OUTPUT);
  pinMode(OUTPUT_PIN_4, OUTPUT);
  
  digitalWrite(OUTPUT_PIN_1, HIGH);
  digitalWrite(OUTPUT_PIN_2, HIGH);
  digitalWrite(OUTPUT_PIN_3, HIGH);
  digitalWrite(OUTPUT_PIN_4, HIGH);
  
  MIDI.begin(MIDI_CHANNEL);
  MIDI.setHandleControlChange(onControlChange);
  MIDI.setHandleProgramChange(onProgramChange);
}

/**
 * Loop
 */
void loop() {
  MIDI.read();
}
