#include "settings.h"

AppState settings_set_servo_endpoint_1(bool init) {
  return IN_MENU;
}

AppState settings_set_servo_endpoint_2(bool init) {
  return IN_MENU;
}

void settings_render_servo_endpoint_1() {
  arcada.display->print("Servo EP 1: ");
  arcada.display->print(SharedConfig.servo_angle_endpoint_1);
}

void settings_render_servo_endpoint_2() {
  arcada.display->print("Servo EP 2: ");
  arcada.display->print(SharedConfig.servo_angle_endpoint_2);
}
