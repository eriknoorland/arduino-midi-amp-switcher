#include "CustomAmpSwitcher.h"
#include "State.h"
#include "states/Init.h"
#include "input/Button.h"

// software version number (major.minor.hotfix)
const char VERSION_NUMBER[8] = "0.8.0";

const Button* selectButton = new Button(4, PULLUP);
const Button* storeButton = new Button(11, PULLUP);
const Button* exitButton = new Button(12, PULLUP);

CustomAmpSwitcher::CustomAmpSwitcher() {
  _state = new Init(this);
}

void CustomAmpSwitcher::setState(State* state) {
  _state = state;
}

void CustomAmpSwitcher::setup() {
  
}

void CustomAmpSwitcher::loop() {
  // MIDI.read();

  if(selectButton->uniquePress()) {
    _state->onSelect();
  }
  
  if(storeButton->uniquePress()) {
    _state->onStore();
  }
  
  if(exitButton->uniquePress()) {
    _state->onExit();
  }

  // listen for rotary input
}
