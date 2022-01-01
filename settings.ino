#include "settings.h"
#include "camera.h"

static bool _dirty = false;
static uint8_t _tempval = 0;

SetEndpointState _set_endpoint(bool init, uint8_t init_value) {
  if (init) {
    _tempval = init_value;
    _dirty = true;
  }

  if (_dirty) {
    arcada.display->fillScreen(ARCADA_BLACK);
    arcada.display->setCursor(0, 0);
    arcada.display->setTextSize(1);
    arcada.display->println("Up/Down to adjust value");
    arcada.display->println("A to confirm, B to cancel");
    arcada.display->println("");

    arcada.display->setTextSize(2);
    arcada.display->print(_tempval);
  }

  uint8_t pressed_buttons = readButtons();

  _dirty = pressed_buttons != 0;

  if (pressed_buttons & ARCADA_BUTTONMASK_UP) {
    _tempval += 1;
  }

  if (pressed_buttons & ARCADA_BUTTONMASK_DOWN) {
    _tempval -= 1;
  }

  _tempval = constrain(_tempval, 0, 180);


  if (pressed_buttons & ARCADA_BUTTONMASK_B) {
    resetServo();
    return SET_ENDPOINT_CANCEL;
  } else if (pressed_buttons & ARCADA_BUTTONMASK_A) {
    resetServo();
    SAButtonServo.write(SharedConfig.servo_neutral_angle);
    return SET_ENDPOINT_ACCEPT;
  } else {
    SAButtonServo.write(_tempval);
    return SET_ENDPOINT_INPROG;
  }
}

AppState settings_set_servo_endpoint_1(bool init) {
  SetEndpointState st = _set_endpoint(init, SharedConfig.servo_angle_endpoint_1);

  switch(st) {
    case SET_ENDPOINT_ACCEPT:
      SharedConfig.servo_angle_endpoint_1 = _tempval;
      config_save(&SharedConfig);

    case SET_ENDPOINT_CANCEL:
      return IN_MENU;
      break;

    case SET_ENDPOINT_INPROG:
      return IN_CALLBACK;

    default:
      return IN_MENU;
  };
}

AppState settings_set_servo_endpoint_2(bool init) {
  SetEndpointState st = _set_endpoint(init, SharedConfig.servo_angle_endpoint_2);

  switch(st) {
    case SET_ENDPOINT_ACCEPT:
      SharedConfig.servo_angle_endpoint_2 = _tempval;
      config_save(&SharedConfig);

    case SET_ENDPOINT_CANCEL:
      return IN_MENU;
      break;

    case SET_ENDPOINT_INPROG:
      return IN_CALLBACK;

    default:
      return IN_MENU;
  };
}

AppState settings_set_servo_neutral(bool init) {
  SetEndpointState st = _set_endpoint(init, SharedConfig.servo_neutral_angle);

  switch(st) {
    case SET_ENDPOINT_ACCEPT:
      SharedConfig.servo_neutral_angle = _tempval;
      config_save(&SharedConfig);

    default:
    case SET_ENDPOINT_CANCEL:
      // if we don't do this then after setting a new neutral the servo
      // resets to old neutral
      resetServo();
      return IN_MENU;
      break;

    case SET_ENDPOINT_INPROG:
      return IN_CALLBACK;
  };
}

AppState settings_servo_stress_test(bool init) {
  static unsigned long last_t = 0;

  if (init) {
    arcada.display->fillScreen(ARCADA_BLACK);
    arcada.display->setCursor(0, 0);
    arcada.display->setTextSize(1);
    arcada.display->println("Press any button to stop");

    last_t = millis();
  }

  unsigned dt = millis() - last_t;

  if (dt < 10000) {
    SAButtonServo.write(SharedConfig.servo_angle_endpoint_1);
  } else if (dt < 20000) {
    SAButtonServo.write(SharedConfig.servo_angle_endpoint_2);
  } else {
    last_t = millis();
  }

  if (readButtons()) {
    resetServo();
    return IN_MENU;
  } else {
    return IN_CALLBACK;
  }
}

AppState settings_camera_trigger_test(bool init) {
  if (init) {
    arcada.display->fillScreen(ARCADA_BLACK);
    arcada.display->setCursor(0, 0);
    arcada.display->setTextSize(1);
    arcada.display->println("Press any button to release");
    camera_press_release();
  }

  if (readButtons()) {
    camera_press_release();
    return IN_MENU;
  } else {
    return IN_CALLBACK;
  }
}

void settings_render_servo_endpoint_1() {
  arcada.display->print("Servo EP 1: ");
  arcada.display->print(SharedConfig.servo_angle_endpoint_1);
}

void settings_render_servo_endpoint_2() {
  arcada.display->print("Servo EP 2: ");
  arcada.display->print(SharedConfig.servo_angle_endpoint_2);
}

void settings_render_servo_neutral() {
  arcada.display->print("Servo Neutral: ");
  arcada.display->print(SharedConfig.servo_neutral_angle);
}

