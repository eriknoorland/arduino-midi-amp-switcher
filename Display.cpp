#include "Adafruit_LiquidCrystal.h"
#include "Display.h"

Adafruit_LiquidCrystal lcd(0);

void Display::update(String line1 = "", String line2 = "", boolean clear = false) {
  if(clear) {
    lcd.clear();
  }

  this->updateLine(line1, 0);
  this->updateLine(line2, 1);
}

void Display::updateLine(String body = "", byte lineNumber = 0) {
  if(body != "") {
    lcd.setCursor(0, lineNumber);
    lcd.print("                ");
    lcd.setCursor(0, lineNumber);
    lcd.print(body);
  }
}
