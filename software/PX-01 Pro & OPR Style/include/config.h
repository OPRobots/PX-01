#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <motors.h>
#include <pinout.h>

/**
 * @brief Indica el modo de funcionamiento del robot.
 *
 */
#define MODE_TEST 1
// #define MODE_RACE 2

/**
 * @brief Indica si la línea a seguir es blanca o negra.
 * Este control se usa para aplicar un filtro en la detección de la línea.
 * 
 */
// #define LINE_WHITE 0
#define LINE_BLACK 1

/**
 * @brief Tiempo de espera para iniciar carrera después de pulsar el botón.
 *
 */
#define START_TEST_MS 3000
#define START_RACE_MS 5000

/**
 * @brief Tiempo máximo de ejecución en modo prueba (MODE_TEST).
 * Después de este tiempo, el robot se detendrá automáticamente.
 * 
 */
#define TEST_DURATION_MS 5000

/**
 * @brief Define el tiempo de espera entre ejecuciones del bucle de control en microsegundos.
 *
 */
#define CONTROL_LOOP_US 1000

/**
 * @brief Tiempo de calibración de sensores.
 *
 */
#define SENSORS_CALIBRATION_MS 3000

/**
 * @brief Configuración de Velocidad y Aceleración.
 * Dispone de varios menús para seleccionar la velocidad y aceleración del robot.
 * 
 */
#define MENU_1_BASE_SPEED 20
#define MENU_1_ACCEL_SPEED 60
#define MENU_1_BASE_FAN_SPEED 85

#define MENU_2_BASE_SPEED 35
#define MENU_2_ACCEL_SPEED 60
#define MENU_2_BASE_FAN_SPEED 85

#define MENU_3_BASE_SPEED 50
#define MENU_3_ACCEL_SPEED 60
#define MENU_3_BASE_FAN_SPEED 85

/**
 * @brief Constantes del controlador PID.
 *
 */
#define PID_KP 0.2
#define PID_KD 0.80

void init_components();
long get_ms_start();

#endif