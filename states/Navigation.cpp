#include "CustomAmpSwitcher.h"
#include "State.h"
#include "Navigation.h"
#include "Init.h"
// #include "Display.h"

void Navigation::init() {
  // Display::getInstance()->update("CHANNEL <", "PROGRAM", true);
}

void Navigation::onExit() {
  _context->setState(new Init(this->_context));
}
