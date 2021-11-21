#include <Adafruit_Arcada.h>
#include <Servo.h>

#include "types.h"
#include "menu.h"
#include "about.h"
#include "valign.h"
#include "config.h"
#include "settings.h"
#include "pin_defs.h"

Adafruit_Arcada arcada;

const uint8_t MAX_BACKLIGHT = 128;

AppState _AppState = IN_MENU;
Config SharedConfig;

Servo SAButtonServo;

AppState show_valign_menu(bool init) {
  menu_push(&valign_menu);
  return IN_MENU;
}

AppState show_settings_menu(bool init) {
  menu_push(&settings_menu);
  return IN_MENU;
}

Menu main_menu = {
  {
    // text, on_select, render
    {"Do V-Align", show_valign_menu, NULL},
    {"Take Dark Frames", NULL, NULL},
    {"Take Light Frames", NULL, NULL},
    {"Settings", show_settings_menu, NULL},
    {"About", show_about, NULL},
  },
  5,
  0
};
bool _Dirty = true;

void resetServo() {
  SAButtonServo.write(SharedConfig.servo_neutral_angle);
}

uint8_t readButtons() {
  arcada.readButtons();
  delay(20);
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

  config_load(&SharedConfig);

  menu_init(&main_menu);

  SAButtonServo.attach(SA_BUTTON_SERVO_PIN);
  SAButtonServo.write(SharedConfig.servo_neutral_angle);

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

