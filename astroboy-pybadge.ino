#include <Adafruit_Arcada.h>
#include <Servo.h>

#include "types.h"
#include "menu.h"
#include "about.h"
#include "valign.h"
#include "config.h"
#include "settings.h"
#include "pin_defs.h"
#include "camera.h"
#include "capture.h"

Adafruit_Arcada arcada;

const uint8_t MAX_BACKLIGHT = 128;

AppState _AppState = IN_MENU;
Config SharedConfig;

Servo SAButtonServo;

Menu main_menu = {
  {
    // text, on_select, render, submenu
    {"Do D.A.R.V.",        NULL,       NULL,  &valign_menu},
    {"Do Captures",       NULL,       NULL,   &capture_menu},
    {"Settings",          NULL,       NULL,   &settings_menu},
    {"About",             show_about, NULL,   NULL},
  },
  5,
  0
};
bool _Dirty = true;

// TODO: move into servo module
void servo_reset() {
  SAButtonServo.write(SharedConfig.servo_neutral_angle);
}

/**
 * Returns true if endpoint is known, false otherwise
 */
bool servo_go_endpoint(uint8_t ep) {
  if (ep == 1) {
    SAButtonServo.write(SharedConfig.servo_angle_endpoint_1);
    return true;
  } else if (ep == 2) {
    SAButtonServo.write(SharedConfig.servo_angle_endpoint_2);
    return true;
  } else {
    return false;
  }
}

// TODO: rename and move into input module
uint8_t readButtons() {
  arcada.readButtons();
  return arcada.justPressedButtons();
}

void setup() {
  if (arcada.arcadaBegin()) {
    arcada.pixels.setPixelColor(0, arcada.pixels.Color(0, 255, 0));
    arcada.pixels.show();
    delay(500);
  } else {
    arcada.pixels.setPixelColor(0, arcada.pixels.Color(255, 0, 0));
    arcada.pixels.show();
    for(;;);
  }

  arcada.displayBegin();

  // ramp up the backlight
  for (int i=0; i<MAX_BACKLIGHT; i++) {
    //Serial.printf("lite = %d\n", i);
    arcada.setBacklight(i);
    delay(1);
  }

  arcada.display->fillScreen(ARCADA_BLACK);
  arcada.display->setCursor(0, 0);
  arcada.display->setTextColor(ARCADA_RED);
  arcada.display->setTextSize(2);
  arcada.display->println("ASTROBOY");

  arcada.display->setTextSize(1);

  config_init(&SharedConfig);

  menu_init(&main_menu);

  SAButtonServo.attach(SA_BUTTON_SERVO_PIN);
  SAButtonServo.write(SharedConfig.servo_neutral_angle);

  camera_init();

  arcada.display->println("Press any button");
  while (readButtons() == 0);

  arcada.display->fillScreen(ARCADA_BLACK);
}

void loop() {
  switch(_AppState) {
    case IN_MENU:
      _AppState = menu_draw();
      break;

    case IN_CALLBACK:
      _AppState = menu_callback(false);
      break;

    default:
      arcada.display->fillScreen(ARCADA_BLACK);
      arcada.display->setCursor(0, 0);
      arcada.display->print("Unknown _AppState");
      break;
  }
}

