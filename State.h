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
    
    bool getLoopRotaryValue();
    int getMinRotaryValue();
    int getMaxRotaryValue();

    virtual void init() {};
    virtual void onScroll(int value) {};
    virtual void onSelect() {};
    virtual void onStore() {};
    virtual void onExit() {};
};

#endif
