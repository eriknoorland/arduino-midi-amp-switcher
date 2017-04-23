#ifndef INIT_H
#define INIT_H

class Init: public State {
  public:
    Init(CustomAmpSwitcher* context): State(context) {}
};

#endif
