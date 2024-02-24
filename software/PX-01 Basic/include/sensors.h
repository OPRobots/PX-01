#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <config.h>
#include <pinout.h>
#include <utils.h>

/**
 * @brief Cantidad de sensores.
 *
 */
#define SENSORS_COUNT 16

/**
 * @brief Valor máximo y mínimo.
 *
 */
#define SENSORS_MAX 4095
#define SENSORS_MIN 0


/**
 * @brief Valor máximo de la posición sobre la línea.
 * El rango de valores será de -SENSORS_POSITION_MAX a SENSORS_POSITION_MAX, con 0 como el punto central.
 *
 */
#define SENSORS_POSITION_MAX 255

void calibrate_sensors();
int get_sensor_raw(int sensor);
int get_sensor_calibrated(int sensor);
int get_sensor_position(int last_position);
long get_last_line_detected_ms();

#endif // SENSORS_H