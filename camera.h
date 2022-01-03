#ifndef CAMERA_H
#define CAMERA_H

void camera_init();
void camera_press();
void camera_release();
void camera_press_release();

/**
 * Start an exposure, taking into account camera_mirror_lockup_time_s. Returns
 * once exposure has started.
 */ 
void camera_start_exposure(bool verbose); 

/**
 * Ends an exposures, takes into account camera_write_time_s. Returns once
 * data has been stored.
 */ 
void camera_end_exposure(bool verbose);
#endif
