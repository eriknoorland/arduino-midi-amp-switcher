#include "MIDI.h"

#define MIDI_CHANNEL 2

#define NUM_PROGRAM_PRESETS 16
#define NUM_DEVICE_OUTPUTS 4

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
const byte DEVICE_OUTPUT_PINS[NUM_DEVICE_OUTPUTS] = {
  OUTPUT_PIN_1,
  OUTPUT_PIN_2,
  OUTPUT_PIN_3,
  OUTPUT_PIN_4
};

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
 * Program change handler
 * @param {byte} channel
 * @param {byte} program
 */
void onProgramChange(byte channel, byte program) {
  if (channel == MIDI_CHANNEL) {
    String preset = PROGRAM_PRESET_STRINGS[program];

    for (byte i = 0; i < 4; i++) {
      digitalWrite(DEVICE_OUTPUT_PINS[i], (String(preset[i]) == "1" ? LOW : HIGH));
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
  MIDI.setHandleProgramChange(onProgramChange);
}

/**
 * Loop
 */
void loop() {
  MIDI.read();
}
