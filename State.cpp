#include "CustomAmpSwitcher.h"
#include "State.h"

State::State(CustomAmpSwitcher* context): _context(context) {};
    
bool State::getLoopRotaryValue() {
  return this->loopRotaryValue;
}

int State::getMinRotaryValue() {
  return this->minRotaryValue;
}

int State::getMaxRotaryValue() {
  return this->maxRotaryValue;
}
