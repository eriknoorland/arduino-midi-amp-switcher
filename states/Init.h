#ifndef INIT_H
#define INIT_H

class Init: public State {
  public:
    Init(CustomAmpSwitcher* context) : State(context) {}

    void onScroll(int value);
    void onSelect();
    void onStore();
    void onExit();
};

#endif
