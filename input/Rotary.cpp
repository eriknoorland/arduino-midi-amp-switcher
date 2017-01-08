#include "Rotary.h"




// pinMode(ROTARY_PIN_A, INPUT);
// pinMode(ROTARY_PIN_B, INPUT);
// digitalWrite(ROTARY_PIN_A, HIGH);
// digitalWrite(ROTARY_PIN_B, HIGH);
// attachInterrupt(0, onRotaryUpdatePinA, RISING);
// attachInterrupt(1, onRotaryUpdatePinB, RISING);


// /**
//  * Rotary pin A change handler
//  */
// void onUpdatePinA() {
//   cli();
//   pinReading = PIND & 0xC;

//   if(pinReading == B00001100 && rotaryPinAFlag) {
//     newRotaryValue--;
//     rotaryPinBFlag = 0;
//     rotaryPinAFlag = 0;
//   } else if(pinReading == B00000100) {
//     rotaryPinBFlag = 1;
//   }

//   sei();
// }

// /**
//  * Rotary pin B change handler
//  */
// void onUpdatePinB() {
//   cli();
//   pinReading = PIND & 0xC;

//   if(pinReading == B00001100 && rotaryPinBFlag) {
//     newRotaryValue++;
//     rotaryPinBFlag = 0;
//     rotaryPinAFlag = 0;
//   } else if(pinReading == B00001000) {
//     rotaryPinAFlag = 1;
//   }

//   sei();
// }
