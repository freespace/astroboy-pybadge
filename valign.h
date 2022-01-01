#ifndef VALIGN_H
#define VALIGN_H

#import "types.h"
#import "menu.h"

void valign_render_drift_time();
AppState valign_set_drift_time(bool init);
AppState valign_start(bool init);
AppState valign_start_reversed(bool init);

Menu valign_menu = {
  {
    // text, on_select, render
    {
      NULL,
      valign_set_drift_time,
      valign_render_drift_time},
    {"Start", valign_start, NULL},
    {"Start (reversed)", valign_start_reversed, NULL},
  },
  3,
  0
};

#endif
