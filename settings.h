#ifndef SETTINGS_H
#define SETTINGS_H

#import "types.h"
#import "menu.h"

AppState settings_set_servo_endpoint_1(bool init);
AppState settings_set_servo_endpoint_2(bool init);
AppState settings_set_servo_neutral(bool init);
AppState settings_set_camera_write_time(bool init);
AppState settings_servo_stress_test(bool init);
AppState settings_camera_trigger_test(bool init);

void settings_render_servo_endpoint_2();
void settings_render_servo_endpoint_1();
void settings_render_servo_neutral();
void settings_render_camera_write_time();

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
      NULL,
      settings_set_camera_write_time,
      settings_render_camera_write_time,
    },
    {
      "Servo Stress Test",
      settings_servo_stress_test,
      NULL,
    },
    {
      "Camera Trigger Test",
      settings_camera_trigger_test,
      NULL,
    },
  },
  6,
  0
};

#endif
