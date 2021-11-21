#include "config.h"

static const uint8_t CONFIG_VERSION = 2;
static const char *CONFIG_FILE_NAME = "/config.txt";

void config_set_defaults(Config *conf) {
  conf->valign_drift_time_s = 10;

  conf->servo_neutral_angle = 90;
  conf->servo_angle_endpoint_1 = 80;
  conf->servo_angle_endpoint_2 = 100;

  conf->camera_exposure_time_t = 20;
  conf->camera_num_captures = 10;
  conf->camera_write_time_t = 5;
}

bool config_init(Config *conf) {
  if (arcada.filesysBegin()) {
    arcada.display->println("Found storage");
    File conf_file = arcada.open(CONFIG_FILE_NAME);
    if (!conf_file) {
      arcada.display->println("Creating new config");
      config_set_defaults(conf);
      config_save(conf);
    } else {
      arcada.display->print("Opened ");
      arcada.display->println(CONFIG_FILE_NAME);

      bool success = config_read(&conf_file, conf);
      conf_file.close();

      if (success) {
        arcada.display->println("Loaded config values");
      } else {
        arcada.display->println("Invalid config, using defaults");
        config_set_defaults(conf);
        config_save(conf);
      }
    }

  }

  return true;
}

bool config_read(File *conf_file, Config *conf) {

  uint8_t version = conf_file->parseInt();
  if (version != CONFIG_VERSION) {
    return false;
  }

  conf->valign_drift_time_s = conf_file->parseInt();

  conf->servo_neutral_angle = conf_file->parseInt();
  conf->servo_angle_endpoint_1 = conf_file->parseInt();
  conf->servo_angle_endpoint_2= conf_file->parseInt();

  conf->camera_exposure_time_t = conf_file->parseInt();
  conf->camera_num_captures = conf_file->parseInt();
  conf->camera_write_time_t = conf_file->parseInt();

  return true;
}

bool config_save(Config *conf) {
  // overwrite the existing file, create if required
  File conf_file = arcada.open(CONFIG_FILE_NAME, O_WRITE | O_CREAT);
  bool success = false;
  if (!conf_file) {
    arcada.errorBox("Failed to save config");
  } else {
    conf_file.print(CONFIG_VERSION);
    conf_file.print(" ");

    conf_file.print(conf->valign_drift_time_s);
    conf_file.print(" ");

    conf_file.print(conf->servo_neutral_angle);
    conf_file.print(" ");

    conf_file.print(conf->servo_angle_endpoint_1);
    conf_file.print(" ");

    conf_file.print(conf->servo_angle_endpoint_2);
    conf_file.print(" ");

    conf_file.print(conf->camera_exposure_time_t);
    conf_file.print(" ");

    conf_file.print(conf->camera_num_captures);
    conf_file.print(" ");

    conf_file.print(conf->camera_write_time_t);
    conf_file.print(" ");

    success = true;

    conf_file.flush();
    conf_file.close();
  }

  return success;
}
