#ifndef ROTARY_H
#define ROTARY_H

// #include "Arduino.h"

// #define PULLUP HIGH
// #define PULLDOWN LOW

// #define CURRENT 0
// #define PREVIOUS 1
// #define CHANGED 2


// const byte ROTARY_PIN_A = 2;
// const byte ROTARY_PIN_B = 3;

// volatile byte rotaryPinAFlag = 0;
// volatile byte rotaryPinBFlag = 0;
// volatile int newRotaryValue = 0;
// volatile int prevRotaryValue = 0;
// volatile byte pinReading = 0;

class Rotary {
  private:
    int pinA;
    int pinB;
    void onUpdatePinA();
    void onUpdatePinB();

  public:
    Rotary(int pinA, int pinB);
    // void pullup();
    // void pulldown();
    // bool isPressed();
    // bool wasPressed();
    // bool stateChanged();
    // bool uniquePress();
};

#endif
