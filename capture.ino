#include "capture.h"
#include "camera.h"
#include "input.h"
#include "types.h"
#include "config.h"

AppState capture_start(bool init) {
  static unsigned long next_capture_deadline_ms = 0;
  static unsigned long next_render_deadline_ms = 0;
  static int num_captured = 0;
  static bool done = false;
  static bool capturing = false;

  unsigned long now = millis();

  if (init) {
    next_capture_deadline_ms = 0;
    next_render_deadline_ms = 0;
    num_captured = 0;
    done = false;
    capturing = false;
  }

  // periodically update the display
  if (now >= next_render_deadline_ms) {
    next_render_deadline_ms = now + 500;
    if (done) {
      // reduce rendering frequency
      next_render_deadline_ms += 60 * 1000;
    }

    arcada.display->fillScreen(ARCADA_BLACK);
    arcada.display->setCursor(0, 0);
    arcada.display->setTextSize(1);
    arcada.display->print("Captured: ");
    arcada.display->print(num_captured);
    arcada.display->print("/");
    arcada.display->print(SharedConfig.camera_num_captures);
    arcada.display->println("");

    if (capturing) {
      arcada.display->println("Exp. Time Left:");
      arcada.display->println("");
      arcada.display->setTextSize(2);
      arcada.display->println((next_capture_deadline_ms - now)/1000.0);
    }

    if (done) {
      arcada.display->println("Press any button to exit");
      arcada.display->println("");
      arcada.display->setTextSize(2);
      arcada.display->println("Done");
    }
  }

  done = num_captured >= SharedConfig.camera_num_captures;
  bool capture_deadline_passed = now >= next_capture_deadline_ms;

  if (done) {
    if (readButtons() != 0) {
      return IN_MENU;
    }
  } else {
    if (capturing && capture_deadline_passed) {
      // stop capture
      camera_press_release();

      // wait for camera to write data
      arcada.display->setTextSize(1);
      arcada.display->println("");
      arcada.display->println("Writing data");
      // let the camera write data out
      delay(SharedConfig.camera_write_time_s * 1000);

      // increment the number of captures taken
      num_captured += 1;

      // no longer capturing
      capturing = false;
    } else if (capturing == false) {
      // if we are not done and not capturing, start a new capture
      camera_press_release();

      // set when the capture will stop. This must come after camera_press_release() since capture
      // starts when that function returns
      next_capture_deadline_ms = millis() + SharedConfig.camera_exposure_time_s * 1000;

      // set the fact we are capturing
      capturing = true;
    }
  }

  return IN_CALLBACK;
}

AppState capture_set_exposure_time(bool init) {
  InputState st = input_get_uint8(
      init,
      SharedConfig.camera_exposure_time_s,
      1,
      240,
      &SharedConfig.camera_exposure_time_s);

  switch(st) {
    case INPUT_USER_ACCEPT:
      config_save(&SharedConfig);
    case INPUT_USER_CANCEL:
      return IN_MENU;

    default:
      return IN_CALLBACK;
  }

}

AppState capture_set_count(bool init) {
  InputState st = input_get_uint8(
      init,
      SharedConfig.camera_num_captures,
      1,
      250,
      &SharedConfig.camera_num_captures);

  if (st == INPUT_IN_PROGRESS) {
    return IN_CALLBACK;
  } else {
    if (st == INPUT_USER_ACCEPT) {
      config_save(&SharedConfig);
    }
    return IN_MENU;
  }
}

void capture_render_exposure_time() {
  arcada.display->print("Exposure (s): ");
  arcada.display->print(SharedConfig.camera_exposure_time_s);
}

void capture_render_count() {
  arcada.display->print("Count: ");
  arcada.display->print(SharedConfig.camera_num_captures);
}
