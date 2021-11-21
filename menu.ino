#include "menu.h"

static const uint8_t MenuStackSize = 8;

static Menu * _MenuStack[MenuStackSize];
static uint8_t _MenuStackTop = 0;

static MenuCallback_Ptr _currentCallback;

void menu_init(Menu *mainMenu) {
  _MenuStack[0] = mainMenu;
  _MenuStackTop = 0;
}

bool menu_push(Menu *submenu) {
  if (_MenuStackTop < MenuStackSize) {
    _MenuStackTop += 1;
    _MenuStack[_MenuStackTop] = submenu;
    return true;
  } else {
    return false;
  }
}

bool menu_pop() {
  if (_MenuStackTop > 0) {
    _MenuStackTop -= 1;
    return true;
  } else {
    return false;
  }
}

Menu *menu_get_current() {
  return _MenuStack[_MenuStackTop];
}

AppState menu_draw() {
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
    menu_pop();
    return IN_MENU;
  } else if (pressed_buttons & ARCADA_BUTTONMASK_A) {
    MenuEntry *entry = &(menu->entries[menu->selection_position]);

    if (entry->on_select) {
      _currentCallback = entry->on_select;
      return _currentCallback(true);
    } else if (entry->submenu) {
      menu_push(entry->submenu);
      return IN_MENU;
    } else {
      return IN_MENU;
    }
  } else {
    return IN_MENU;
  }
}

AppState menu_callback(bool init) {
  return _currentCallback(init);
}
