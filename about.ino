#include "about.h"

AppState show_about(bool init) {
  if (init) {
    arcada.display->fillScreen(ARCADA_BLACK);
    arcada.display->setCursor(0, 0);
    arcada.display->println("Astroboy by @freespace");
    arcada.display->println("Version 0.1");
    arcada.display->println("Press B to exit");
  }

  if (readButtons() & ARCADA_BUTTONMASK_B) {
    return IN_MENU;
  } else {
    return IN_CALLBACK;
  }
}
