#ifndef BUTTON_H
#define BUTTON_H

#include "Arduino.h"

#define PULLUP HIGH
#define PULLDOWN LOW

#define CURRENT 0
#define PREVIOUS 1
#define CHANGED 2

class Button {
  private:
    int pin;
    int mode;
    int state;

  public:
    Button(int pin, int mode);
    void pullup();
    void pulldown();
    bool isPressed();
    bool wasPressed();
    bool stateChanged();
    bool uniquePress();
};

#endif
