#ifndef CAPTURE_H
#define CAPTURE_H

#include "types.h"
#include "menu.h"

AppState capture_start(bool init);
AppState capture_set_exposure_time(bool init);
AppState capture_set_count(bool init);

void capture_render_exposure_time();
void capture_render_count();

Menu capture_menu = {
  {
    {
      NULL,
      capture_set_exposure_time,
      capture_render_exposure_time,
    },
    {
      NULL,
      capture_set_count,
      capture_render_count,
    },
    {
      "Start",
      capture_start,
      NULL,
    },
  },
  3,
  0
};

#endif
