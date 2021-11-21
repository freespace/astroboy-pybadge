#ifndef SETTINGS_H
#define SETTINGS_H

#import "types.h"
#import "menu.h"

AppState settings_set_servo_endpoint_1(bool init);
AppState settings_set_servo_endpoint_2(bool init);

void settings_render_servo_endpoint_2();
void settings_render_servo_endpoint_1();

Menu settings_menu = {
  {
    {
      NULL,
      settings_set_servo_endpoint_1,
      settings_render_servo_endpoint_1,
    },
    {
      NULL,
      settings_set_servo_endpoint_2,
      settings_render_servo_endpoint_2,
    },
  },
  2,
  0
};

#endif
