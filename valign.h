#ifndef VALIGN_H
#define VALIGN_H

#import "types.h"
#import "menu.h"

void valign_render_drift_time();
AppState valign_set_drift_time(bool init);

Menu valign_menu = {
  {
    // text, on_select, render
    {
      NULL,
      valign_set_drift_time,
      valign_render_drift_time},
    {"Start", NULL, NULL},
    {"Start (reversed)", NULL, NULL},
  },
  3,
  0
};

#endif
