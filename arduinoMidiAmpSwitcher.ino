#include "CustomAmpSwitcher.h"

CustomAmpSwitcher* switcher;

void setup() {
  Serial.begin(9600);
  Serial.println("setup");
  
  switcher = new CustomAmpSwitcher();
}

void loop() {
  switcher->loop();
}
