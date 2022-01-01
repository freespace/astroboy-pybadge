#include "valign.h"
#include "config.h"

static const uint8_t MAX_DRIFT_TIME_S = 240;

void valign_render_drift_time() {
  arcada.display->print("Drift Time: ");
  arcada.display->print(SharedConfig.valign_drift_time_s);
  arcada.display->print("s");
}

AppState valign_set_drift_time(bool init) {
  static bool dirty = false;
  static uint8_t tempval = 0;

  if (init) {
    dirty = true;
    tempval = SharedConfig.valign_drift_time_s;
  }

  if (dirty) {
    arcada.display->fillScreen(ARCADA_BLACK);
    arcada.display->setCursor(0, 0);
    arcada.display->setTextSize(1);
    arcada.display->println("Up/Down to adjust value");
    arcada.display->println("A to confirm, B to cancel");
    arcada.display->println("");

    arcada.display->setTextSize(2);
    arcada.display->print(tempval);
    arcada.display->print("s");
  }

  uint8_t pressed_buttons = readButtons();

  dirty = pressed_buttons != 0;

  if (pressed_buttons & ARCADA_BUTTONMASK_UP) {
    tempval += 1;
  }

  if (pressed_buttons & ARCADA_BUTTONMASK_DOWN) {
    tempval -= 1;
  }

  tempval = constrain(tempval, 0, MAX_DRIFT_TIME_S);


  if (pressed_buttons & ARCADA_BUTTONMASK_B) {
    return IN_MENU;
  } else if (pressed_buttons & ARCADA_BUTTONMASK_A) {
    SharedConfig.valign_drift_time_s = tempval;
    config_save(&SharedConfig);
    return IN_MENU;
  } else {
    return IN_CALLBACK;
  }
}

static AppState _valign_start(bool init, bool reversed) {
  static unsigned long last_t = 0;
  static bool is_halfway = false;

  if (init) {
    arcada.display->fillScreen(ARCADA_BLACK);
    arcada.display->setCursor(0, 0);
    arcada.display->setTextSize(1);
    arcada.display->println("V-align Start");

    arcada.display->print("Drift time (s): ");
    arcada.display->println(SharedConfig.valign_drift_time_s);

    arcada.display->print("Reversed: ");
    arcada.display->println(reversed);

    arcada.display->println("Dwelling");
    // trigger the camera
    camera_press_release();

    // dwell for 2 seconds for an extra white spot so we know where the start was
    delay(2000);

    last_t  = millis();
    is_halfway = false;

    // start the motion
    if (reversed) {
      servo_go_endpoint(2);
    } else {
      servo_go_endpoint(1);
    }
    arcada.display->println("Moving");
  }

  long dt = millis() - last_t;

  if (dt > SharedConfig.valign_drift_time_s * 1000) {
    // release the camera
    camera_press_release();

    // stop the motion
    servo_reset();

    // and we are done
    return IN_MENU;
  } else if (dt > SharedConfig.valign_drift_time_s * 1000 / 2) {
    if (is_halfway == false) {
      arcada.display->println("Reversing");
      is_halfway = true;
      // half the drift time has passed, go back the other way
      if (reversed) {
        servo_go_endpoint(1);
      } else {
        servo_go_endpoint(2);
      }
    }
    return IN_CALLBACK;
  } else {
    // we are in the first half of the drift and motion was started
    // in init
    return IN_CALLBACK;
  }
}

AppState valign_start(bool init) {
  return _valign_start(init, false);
}

AppState valign_start_reversed(bool init) {
  return _valign_start(init, true);
}
