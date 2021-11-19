#include "valign.h"

static uint8_t _DriftTime_S = 15;

void valign_render_drift_time() {
  arcada.display->print("Drift Time: ");
  arcada.display->print(_DriftTime_S);
  arcada.display->print("s");
}

AppState valign_set_drift_time(bool init) {
  static bool dirty = false;
  static uint8_t tempval = 0;

  if (init) {
    dirty = true;
    tempval = _DriftTime_S;
  }

  if (dirty) {
    arcada.display->fillScreen(ARCADA_BLACK);
    arcada.display->setCursor(0, 0);
    arcada.display->setTextSize(1);
    arcada.display->println("Up/Down to adjust value");
    arcada.display->println("A to confirm, B to cancel");
    arcada.display->println("");

    arcada.display->setTextSize(2);
    arcada.display->print(tempval);
    arcada.display->print("s");
  }

  uint8_t pressed_buttons = readButtons();

  dirty = pressed_buttons != 0;

  if (pressed_buttons & ARCADA_BUTTONMASK_UP) {
    tempval += 1;
  }

  if (pressed_buttons & ARCADA_BUTTONMASK_DOWN) {
    tempval -= 1;
  }

  if (pressed_buttons & ARCADA_BUTTONMASK_B) {
    return IN_MENU;
  } else if (pressed_buttons & ARCADA_BUTTONMASK_A) {
    _DriftTime_S = tempval;
    return IN_MENU;
  } else {
    return IN_CALLBACK;
  }
}
