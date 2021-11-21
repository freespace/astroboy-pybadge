#ifndef MENU_H
#define MENU_H

#include "types.h"

bool menu_push(Menu *submenu);
bool menu_pop();
void menu_init(Menu *mainMenu);
AppState menu_draw();

AppState menu_callback(bool init);
#endif
