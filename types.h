#ifndef TYPES_H
#define TYPES_H
typedef enum {
  IN_MENU,
  IN_CALLBACK,
} AppState;

/** 
 * callback when a menu entry is selected. init is true when the callback is called for the first
 * immediately after user selection and false on subsequent calls in loop().
 * Use this to determine when to init values or draw the screen for the first time.
 *
 * Note that this is called _repeatedly_ until it returns IN_MENU.
*/
typedef AppState (*MenuCallback_Ptr)(bool init);

// callback to get the text for a menu entry
typedef void (*MenuEntryRenderCallback)();

struct Menu_t;

typedef struct {
  char *text;
  MenuCallback_Ptr on_select;
  MenuEntryRenderCallback render;
  struct Menu_t *submenu;
} MenuEntry;


struct Menu_t {
  // this is inefficient b/c not all menus have the same number of entries but we have resources
  // to spare
  MenuEntry entries[8];

  // number of entries
  uint8_t num_entries;

  // position of the current selection
  int8_t selection_position;

};

typedef struct Menu_t Menu;

typedef struct {
  uint8_t valign_drift_time_s;

  uint8_t servo_neutral_angle;
  uint8_t servo_angle_endpoint_1;
  uint8_t servo_angle_endpoint_2;

  uint8_t camera_exposure_time_t;
  uint8_t camera_num_captures;
  uint8_t camera_write_time_t;
} Config;

typedef enum {
  SET_ENDPOINT_CANCEL,
  SET_ENDPOINT_ACCEPT,
  SET_ENDPOINT_INPROG,
} SetEndpointState;

#endif
