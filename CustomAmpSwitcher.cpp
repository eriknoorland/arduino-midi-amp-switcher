#include "CustomAmpSwitcher.h"
#include "State.h"
#include "states/Init.h"
#include "input/Button.h"
#include "input/Rotary.h"
#include "Display.h"

const Button* selectButton = new Button(4, PULLUP);
const Button* storeButton = new Button(11, PULLUP);
const Button* exitButton = new Button(12, PULLUP);
const Rotary* rotaryKnob = new Rotary(2, 3);

State* _state;

/**
 * CustomAmpSwitcher
 */
CustomAmpSwitcher::CustomAmpSwitcher() {
  Serial.println("CustomAmpSwitcher::CustomAmpSwitcher");
  Display::getInstance().update("CUSTOM MIDI", "AMP SWITCHER", true);
  this->setState(new Init(this));
}

/**
 * Sets the app state
 * @param {State} state
 */
void CustomAmpSwitcher::setState(State* state) {
  Serial.println("CustomAmpSwitcher::setState");
  
  this->_state = state;
  this->_state->init();

  rotaryKnob->setLoopValue(this->_state->getLoopRotaryValue());
  rotaryKnob->setMinValue(this->_state->getMinRotaryValue());
  rotaryKnob->setMaxValue(this->_state->getMaxRotaryValue());
}

/**
 * Application loop
 */
void CustomAmpSwitcher::loop() {
  // MIDI.read();

  if(selectButton->uniquePress()) {
    Serial.println("selectButton->uniquePress");
    this->_state->onSelect();
  }
  
  if(storeButton->uniquePress()) {
    this->_state->onStore();
  }
  
  if(exitButton->uniquePress()) {
    this->_state->onExit();
  }

  if(rotaryKnob->onUpdate()) {
    this->_state->onScroll(rotaryKnob->getValue());
  }
}
