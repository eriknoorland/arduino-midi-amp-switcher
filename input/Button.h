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

/**
 * Button
 * @param {int} pin the pin number that the button is connected to
 * @param {int} mode indicates PULLUP or PULLDOWN resistor
 */
Button::Button(int pin, int mode) {
  this->pin = pin;
  pinMode(this->pin, INPUT);
  mode == PULLDOWN ? pulldown() : pullup();

  this->state = 0;
  bitWrite(state, CURRENT, !this->mode);
}

/**
 * Set the pin high by default
 */
void Button::pullup() {
  this->mode = PULLUP;
  digitalWrite(this->pin, HIGH);
}

/**
 * Set the pin low by default
 */
void Button::pulldown() {
  this->mode = PULLDOWN;
  digitalWrite(this->pin, LOW);
}

/**
 * Returns the current state of the button
 * @return {bool}
 */
bool Button::isPressed() {
  bitWrite(this->state, PREVIOUS, bitRead(this->state, CURRENT));

  if(digitalRead(this->pin) == mode){
    bitWrite(this->state, CURRENT, false);
  } else {
    bitWrite(this->state, CURRENT, true);
  }

  if(bitRead(this->state, CURRENT) != bitRead(state, PREVIOUS)){
    bitWrite(this->state, CHANGED, true);
  } else{
    bitWrite(this->state, CHANGED, false);
  }

  return bitRead(this->state, CURRENT);
}

/**
 * Returns whether the button has been pressed
 * @return {bool}
 */
bool Button::wasPressed() {
  return bitRead(this->state, CURRENT);
}

/**
 * Returns whether the state hasn't changed
 * @return {bool}
 */
bool Button::stateChanged() {
  return bitRead(this->state, CHANGED);
}

/**
 * Returns whether the button is pressed and the state hasn't changed
 * @return {bool}
 */
bool Button::uniquePress() {
  return (isPressed() && stateChanged());
}
