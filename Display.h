#ifndef DISPLAY_H
#define DISPLAY_H

class Display {
  private:
    Display();
    static Display* instance;

  public:
    static Display* getInstance() {
      if(!instance) {
        instance = new Display();
      }

      return instance;
    }

    virtual void update(String line1 = "", String line2 = "", boolean clear = false);
    virtual void updateLine(String body = "", byte lineNumber = 0);
};

#endif
