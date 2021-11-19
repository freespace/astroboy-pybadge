#include <Adafruit_Arcada.h>

#include "types.h"
#include "about.h"
#include "valign.h"

Adafruit_Arcada arcada;

const uint8_t MAX_BACKLIGHT = 128;

AppState _AppState = IN_MENU;
MenuCallback_Ptr _currentCallback;
Menu *_MenuStack[8];
uint8_t _MenuStackTop = 0;

Menu _VAlignMenu = {
  {
    // text, on_select, render
    {
      NULL,
      valign_set_drift_time,
      valign_render_drift_time},
    {"Start", NULL, NULL},
  },
  2,
  0
};

AppState show_valign_menu(bool init) {
  _MenuStackTop += 1;
  _MenuStack[_MenuStackTop] = &_VAlignMenu;

  // reset position to 0 which is the expected thing
  _VAlignMenu.selection_position = 0;
  return IN_MENU;
}

Menu _MainMenu = {
  {
    // text, on_select, render
    {"Do V-Align", show_valign_menu, NULL},
    {"Take Dark Frames", NULL, NULL},
    {"Take Light Frames", NULL, NULL},
    {"About", show_about, NULL},
  },
  4,
  0
};
bool _Dirty = true;

uint8_t readButtons() {
  arcada.readButtons();
  delay(20);
  return arcada.justPressedButtons();
}

void setup() {
  // put your setup code here, to run once:
  if (arcada.arcadaBegin()) {
    arcada.pixels.setPixelColor(0, arcada.pixels.Color(0, 255, 0));
    arcada.pixels.show();
    delay(500);
  } else {
    arcada.pixels.setPixelColor(0, arcada.pixels.Color(255, 0, 0));
    arcada.pixels.show();
    for(;;);
  }

  arcada.displayBegin();

  // ramp up the backlight
  for (int i=0; i<MAX_BACKLIGHT; i++) {
    //Serial.printf("lite = %d\n", i);
    arcada.setBacklight(i);
    delay(1);
  }

  arcada.display->fillScreen(ARCADA_BLACK);
  arcada.display->setCursor(0, 0);
  arcada.display->setTextColor(ARCADA_RED);
  arcada.display->setTextSize(2);
  arcada.display->print("ASTROBOY");

  delay(1000);

  arcada.display->setTextSize(1);
  arcada.display->fillScreen(ARCADA_BLACK);

  _MenuStack[0] = &_MainMenu;
}

void loop() {
  switch(_AppState) {
    case IN_MENU:
      _AppState = drawCurrentMenu();
      break;

    case IN_CALLBACK:
      _AppState = _currentCallback(false);
      break;

    default:
      arcada.display->fillScreen(ARCADA_BLACK);
      arcada.display->setCursor(0, 0);
      arcada.display->print("Unknown _AppState");
      break;
  }
}

AppState drawCurrentMenu() {
  Menu *menu = _MenuStack[_MenuStackTop];

  if (_Dirty) {
    _Dirty = false;
    arcada.display->fillScreen(ARCADA_BLACK);
    arcada.display->setTextSize(1);

    for (int idx = 0; idx < menu->num_entries; ++idx) {
      MenuEntry *entry = &menu->entries[idx];

      arcada.display->setCursor(1, idx*10);
      if (idx == menu->selection_position) {
        arcada.display->print((char)0x10);
      } else {
        arcada.display->print((char)0x00);
      }

      if (entry->render) {
        entry->render();
      } else {
        arcada.display->print(entry->text);
      }
    }
  }

  uint8_t pressed_buttons = readButtons();

  if (pressed_buttons & ARCADA_BUTTONMASK_UP) {
    menu->selection_position -=1;
  }

  if (pressed_buttons & ARCADA_BUTTONMASK_DOWN) {
    menu->selection_position += 1;
  }

  menu->selection_position = constrain(menu->selection_position, 0, menu->num_entries-1);

  if (pressed_buttons) {
    _Dirty = true;
    delay(250);
  }

  // let's say going back has precedence
  if (pressed_buttons & ARCADA_BUTTONMASK_B) {
    if (_MenuStackTop > 0) {
      _MenuStackTop -= 1;
    }
    return IN_MENU;
  } else if (pressed_buttons & ARCADA_BUTTONMASK_A) {
    _currentCallback = menu->entries[menu->selection_position].on_select;
    return _currentCallback(true);
  } else {
    return IN_MENU;
  }
}
