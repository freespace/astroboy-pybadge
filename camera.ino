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

void camera_start_exposure(bool verbose) {
    if (SharedConfig.camera_mirror_lockup_time_s > 0) {
      if (verbose) {
        arcada.display->println("Engaging Mirror Lockup");
      }

      // mirror lockup is enabled, so we press-release to lock the mirror
      camera_press_release();

      // then wait for the configured amount of time for vibrations due to
      // mirror-slap to go away. This also lets mount vibrations to die away
      delay(SharedConfig.camera_mirror_lockup_time_s * 1000);
    }

    // start the capture, assuming camera is in bulb mode
    camera_press();

    // this is a hack to take into account the delay b/t btn press and exposure
    // starting on my camera. This could be a setting variable but it probably
    // doesn't matter given we normally do exposures of >10 seconds 100ms here
    // and there won't hurt
    delay(110);
}

void camera_end_exposure(bool verbose) {
  // stop exposure assuming camera in bulb mode
  camera_release();

  // wait for the camera to write out
  if (verbose) {
      arcada.display->println("Writing data");
  }

  delay(SharedConfig.camera_write_time_s * 1000);
}

