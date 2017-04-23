#ifndef NAVIGATION_H
#define NAVIGATION_H

class Navigation: public State {
  public:
    Navigation(CustomAmpSwitcher* context): State(context) {
    	maxRotaryValue = 5;
    }
};

#endif
