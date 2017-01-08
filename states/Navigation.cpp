#include "CustomAmpSwitcher.h"
#include "State.h"
#include "Navigation.h"
#include "Init.h"

void Navigation::onExit() {
  _context->setState(new Init(_context));
}

void Navigation::onScroll(int value) {}
void Navigation::onSelect() {}
void Navigation::onStore() {}
