#ifndef ROTARY_H
#define ROTARY_H

#include "Arduino.h"

class Rotary {
  private:
    int pinA;
    int pinB;

    int pinAFlag = 0;
    int pinBFlag = 0;
    int pinReading = 0;

    bool loopValue = false;
    int minValue = 0;
    int maxValue = 0;
    
    int newValue = 0;
    int prevValue = 0;
    int value = 0;

    void onUpdatePinA();
    void onUpdatePinB();

  public:
    Rotary(int pinA, int pinB);
    void setLoopValue(bool value);
    void setMinValue(int value);
    void setMaxValue(int value);
    int getValue();
    bool onUpdate();
};

#endif

/**
 * Button
 * @param {int} pinA the pin number that pin A is connected to
 * @param {int} pinB the pin number that pin B is connected to
 */
Rotary::Rotary(int pinA, int pinB) {
  this->pinA = pinA;
  this->pinB = pinB;

  pinMode(this->pinA, INPUT);
  pinMode(this->pinB, INPUT);

  digitalWrite(this->pinA, HIGH);
  digitalWrite(this->pinB, HIGH);

  // https://github.com/0xPIT/encoder/tree/master
  // attachInterrupt(0, this->onUpdatePinA, RISING);
  // attachInterrupt(1, this->onUpdatePinB, RISING);
}

/**
 * Sets whether the rotary value should loop when it hits the min or max value
 * @param {bool} value
 */
void Rotary::setLoopValue(bool value) {
  this->loopValue = value;
}

/**
 * Sets the rotary min value
 * @param {int} value
 */
void Rotary::setMinValue(int value) {
  this->minValue = value;
}

/**
 * Sets the rotary max value
 * @param {int} value
 */
void Rotary::setMaxValue(int value) {
  this->maxValue = value;
}

/**
 * Returns the current value
 * @return {int}
 */
int Rotary::getValue() {
  return this->value;
}

/**
 * Rotary pin A change handler
 */
void Rotary::onUpdatePinA() {
  cli();

  this->pinReading = PIND & 0xC;

  if(this->pinReading == B00001100 && this->pinAFlag) {
    this->newValue--;
    this->pinBFlag = 0;
    this->pinAFlag = 0;
  } else if(this->pinReading == B00000100) {
    this->pinBFlag = 1;
  }

  sei();
}

/**
 * Rotary pin B change handler
 */
void Rotary::onUpdatePinB() {
  cli();

  this->pinReading = PIND & 0xC;

  if(this->pinReading == B00001100 && this->pinBFlag) {
    this->newValue++;
    this->pinBFlag = 0;
    this->pinAFlag = 0;
  } else if(this->pinReading == B00001000) {
    this->pinAFlag = 1;
  }

  sei();
}

/**
 * Returns if there was a value change
 * @return {bool}
 */
bool Rotary::onUpdate() {
  if(this->prevValue == this->newValue) {
    return;
  }

  this->prevValue = this->newValue;

  // when hitting the maximum value either stop
  // or continue from the minimum value
  if(this->newValue >= this->maxValue) {
    this->newValue = this->loopValue ? this->minValue : this->maxValue - 1;
  }

  // when hitting the minimum value either stop
  // or continue from the maximum value
  if(this->newValue <= this->minValue - 1) {
    this->newValue = this->loopValue ? this->maxValue - 1 : this->minValue;
  }

  // no value change, no need for updating
  if(this->newValue == this->value) {
    return;
  }

  // set the new value
  this->value = this->newValue;

  return this->prevValue != this->value;
}
