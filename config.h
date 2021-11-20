#ifndef CONFIG_H
#define CONFIG_H

#include "types.h"

/**
 * Loads config from flash
 */
bool config_load(Config *conf);

/**
 * Saves config to flash. No other files can be open.
 */
bool config_save(Config *conf);
#endif
