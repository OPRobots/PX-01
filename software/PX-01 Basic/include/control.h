#ifndef CONTROL_H
#define CONTROL_H

#include <config.h>
#include <motors.h>
#include <sensors.h>

void set_race_started(bool started);
bool is_race_started();
long get_race_started_ms();
long get_race_stopped_ms();

void set_base_speed(int speed);
void set_base_accel_speed(int accel_speed);

void control_loop();

#endif