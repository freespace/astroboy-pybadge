#include "capture.h"
#include "camera.h"
#include "input.h"
#include "types.h"
#include "config.h"

AppState capture_start(bool init) {
  return IN_MENU;
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
