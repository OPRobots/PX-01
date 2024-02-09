#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>
#include <pinout.h>

/**
 * @brief Configuración de los motores de tracción.
 * Canales: 0 a 3
 * Frecuencia: 1 kHz
 * Resolución: 10 bits
 * Rango: 0 a 1023
 *
 */
#define PWM_MOTOR_RIGHT_A 0
#define PWM_MOTOR_RIGHT_B 1
#define PWM_MOTOR_LEFT_A 2
#define PWM_MOTOR_LEFT_B 3
#define PWM_MOTORS_HZ 1000
#define PWM_MOTORS_RESOLUTION 10
#define PWM_MOTORS_MAX 1023
#define PWM_MOTORS_MIN 0

/**
 * @brief Configuración del motor de succión.
 * Canal: 4
 * Frecuencia: 50 Hz
 * Resolución: 11 bits
 * Rango: 102 a 204 (1000us a 2000us del Protocolo Servo)
 *
 */
#define PWM_SUCTION 4
#define PWM_SUCTION_HZ 50
#define PWM_SUCTION_RESOLUTION 11
#define PWM_SUCTION_MAX 204
#define PWM_SUCTION_MIN 102

void init_motors();
void set_motors_speed(float velI, float velD);
void set_fan_speed(int vel);

#endif // MOTORS_H