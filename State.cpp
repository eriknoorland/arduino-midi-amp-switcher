#include "CustomAmpSwitcher.h"
#include "State.h"

State::State(CustomAmpSwitcher* context): _context(context) {};

void State::onScroll(int value) {}
void State::onSelect() {}
void State::onStore() {}
void State::onExit() {}
