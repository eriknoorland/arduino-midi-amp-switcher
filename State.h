#ifndef STATE_H
#define STATE_H

class CustomAmpSwitcher;

class State {
  protected:
    CustomAmpSwitcher* _context;

    bool loopRotaryValue = false;
    int minRotaryValue = 0;
    int maxRotaryValue = 0;

  public:
    State(CustomAmpSwitcher* context);
    
    bool getLoopRotaryValue() {
      return loopRotaryValue;
    }

    int getMinRotaryValue() {
      return minRotaryValue;
    }
    
    int getMaxRotaryValue() {
      return maxRotaryValue;
    }

    virtual void onScroll(int value) = 0;
    virtual void onSelect() = 0;
    virtual void onStore() = 0;
    virtual void onExit() = 0;
};

#endif
