#include "CustomAmpSwitcher.h"
#include "State.h"
#include "states/Init.h"
#include "states/Navigation.h"

void Init::onSelect() {
  _context->setState(new Navigation(_context));
}

void Init::onScroll(int value) {}
void Init::onStore() {}
void Init::onExit() {}
