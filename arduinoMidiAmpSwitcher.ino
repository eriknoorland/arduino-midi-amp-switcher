#include <MIDI.h>

int midiChannel = 2; // or use MIDI_CHANNEL_OMNI for all channels

int activityLedPin = 13;
int greenChannelPin = 2;
int redChannelPin = 3;
int boostPin = 4;
int loopPin= 5;

int outputPinCount = 5;
int outputPins[5] = {
  activityLedPin,
  greenChannelPin,
  redChannelPin,
  boostPin,
  loopPin
};

int channelPinCount = 2;
int channelPins[2] = {
  greenChannelPin,
  redChannelPin
};

MIDI_CREATE_DEFAULT_INSTANCE();

/**
 * Program setup
 */
void setup() {
  // setup output pin modes
  for(int i = 0; i < outputPinCount; i++) {
    pinMode(outputPins[i], OUTPUT);
  }

  // setup midi
  MIDI.begin(midiChannel);
  MIDI.setHandleProgramChange(onProgramChange);
}

/**
 * Program loop
 */
void loop() {
  // Keep listening for midi input
  MIDI.read();
}

/**
 * Switches to an amp channel based on the program number
 * @param {byte} number
 */
void switchChannel(byte number) {
  // turn off all channels (which defaults to the orange channel)
  for(int i = 0; i < channelPinCount; i++) {
    digitalWrite(channelPins[i], HIGH);
  }

  // turn on green channel
  if(number == 21) {
    digitalWrite(greenChannelPin, LOW);
  }
  
  // turn on red channel
  if(number == 23) {
    digitalWrite(redChannelPin, LOW);
  }
}

/**
 * Flashes the activity LED
 */
void onMidiActivity() {
  digitalWrite(activityLedPin, HIGH);
  delay(50);
  digitalWrite(activityLedPin, LOW);
  delay(50);
  digitalWrite(activityLedPin, HIGH);
  delay(50);
  digitalWrite(activityLedPin, LOW);
}

/**
 * Midi program change handler
 * @param {byte} channel
 * @param {byte} number
 */
void onProgramChange(byte channel, byte number) {
  Serial.print(number);

  switch(number) {
    case 21:
    case 22:
    case 23:
      switchChannel(number);
      break;
    case 24:
      // toggle boost - boostPin
      break;
    case 25:
      // toggle FX loop - loopPin
      break;
  }
  
  onMidiActivity();
}
