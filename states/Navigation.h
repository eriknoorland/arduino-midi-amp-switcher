#ifndef NAVIGATION_H
#define NAVIGATION_H

class Navigation: public State {
  public:
    Navigation(CustomAmpSwitcher* context) : State(context) {
    	maxRotaryValue = 5;
    }

    void onScroll(int value);
    void onSelect();
    void onStore();
    void onExit();
};

#endif
