#include "input.h"

InputState input_get_uint8(bool init, uint8_t init_val, uint8_t min_val, uint8_t max_val, uint8_t *out_ptr) {
  static bool dirty = true;
  static uint8_t tempval = 0;

  if (init) {
    dirty = true;
    tempval = init_val;
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
  }

  uint8_t pressed_buttons = readButtons();
  dirty = pressed_buttons != 0;

  // TODO: implement press-and-hold-to-increment
  if (pressed_buttons & ARCADA_BUTTONMASK_UP) {
    if (tempval < max_val) {
      tempval += 1;
    }
  }

  if (pressed_buttons & ARCADA_BUTTONMASK_DOWN) {
    if (tempval > min_val) {
      tempval -= 1;
    }
  }

  if (pressed_buttons & ARCADA_BUTTONMASK_B) {
    return INPUT_USER_CANCEL;
  } else if (pressed_buttons & ARCADA_BUTTONMASK_A) {
    if (out_ptr) {
      *out_ptr = tempval;
    }
    return INPUT_USER_ACCEPT;
  } else {
    return INPUT_IN_PROGRESS;
  }
}
