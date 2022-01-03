#include "jog.h"

AppState jog_start(bool init) {
  if (init) {
    arcada.display->fillScreen(ARCADA_BLACK);
    arcada.display->setCursor(0, 0);
    arcada.display->println("Use UP and DOWN buttons");
    arcada.display->println("to jog RA");
    arcada.display->println("");
    arcada.display->println("Press B to exit");
  }

  uint8_t pressed_buttons = arcada.readButtons();

  // use of else-if here b/c only one end point can be active
  // at any one time
  if (pressed_buttons & ARCADA_BUTTONMASK_UP) {
    servo_go_endpoint(1);
  } else if (pressed_buttons & ARCADA_BUTTONMASK_DOWN) {
    servo_go_endpoint(2);
  } else {
    servo_reset();
  }

  if (arcada.justPressedButtons() & ARCADA_BUTTONMASK_B) {
    return IN_MENU;
  } else {
    return IN_CALLBACK;
  }
}
