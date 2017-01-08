#ifndef CUSTOMAMPSWITCHER_H
#define CUSTOMAMPSWITCHER_H

#include "CustomAmpSwitcher.h"
#include "State.h"

class CustomAmpSwitcher {
  private:
    State* _state;

  public:
    CustomAmpSwitcher();
    void setState (State* state);
    void setup();
    void loop();
};

#endif
