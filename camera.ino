#include "camera.h"
#include "pin_defs.h"

void camera_init() {
  pinMode(CAMERA_TRIGGER_PIN, OUTPUT);
  camera_release();
}

void camera_press_release() {
  camera_press();
  delay(500);
  camera_release();
}

void camera_press() {
  digitalWrite(CAMERA_TRIGGER_PIN, HIGH);
}

void camera_release() {
  digitalWrite(CAMERA_TRIGGER_PIN, LOW);
}

