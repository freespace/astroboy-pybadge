#ifndef INPUT_H
#define INPUT_H

#include "types.h"

/**
 * Renders a screen for user input with starting value of init_val and bounded
 * between min_val and max_val. On user pressing A the variable pointed to by
 * out_ptr will be updated to the value shown on the screen. If the user aborts
 * by pressing B nothing is done.
 *
 * Note that if this is used to modif config values you should call config_save()
 * on INPUT_USER_ACCEPT.
 */
InputState input_get_uint8(
    bool init,
    uint8_t init_val,
    uint8_t min_val,
    uint8_t max_val,
    uint8_t *out_ptr);
#endif
