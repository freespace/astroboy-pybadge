#ifndef SETTINGS_H
#define SETTINGS_H

#import "types.h"
#import "menu.h"

AppState settings_set_servo_endpoint_1(bool init);
AppState settings_set_servo_endpoint_2(bool init);
AppState settings_set_servo_neutral(bool init);
AppState settings_servo_stress_test(bool init);

void settings_render_servo_endpoint_2();
void settings_render_servo_endpoint_1();
void settings_render_servo_neutral();

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
    {
      NULL,
      settings_set_servo_neutral,
      settings_render_servo_neutral,
    },
    {
      "Servo Stress Test",
      settings_servo_stress_test,
      NULL,
    },
  },
  4,
  0
};

#endif
