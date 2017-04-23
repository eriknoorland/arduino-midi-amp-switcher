#include "states/Init.h"
#include "states/Navigation.h"
#include "Display.h"

void Init::init() {
  Serial.println("Init::init");
  Display::getInstance().update("CUSTOM MIDI", "AMP SWITCHER", true);
}

void Init::onScroll(int value) {}

void Init::onSelect() {
  Serial.println("Init::onSelect");
  this->_context->setState(new Navigation(this->_context));
}

void Init::onStore() {}

void Init::onExit() {}
