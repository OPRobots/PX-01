#include <control.h>

static long last_control_loop_us = 0;
static int position = 0;
static int last_error = 0;

static int base_speed = 0;
static int base_accel_speed = 0;
static int base_fan_speed = 0;

static int speed = 0;

static bool race_started = false;
static long race_started_ms = 0;
static long race_stopped_ms = 0;

/**
 * @brief Realiza el cálculo de la corrección del controlador PID en función del error respecto a la posición de la línea.
 *
 * @param error Desplazamiento del robot respecto a la línea.
 * @return float Corrección del controlador PID.
 */
static float calc_correction(int error) {
  float p = PID_KP * error;
  float d = PID_KD * (error - last_error);
  last_error = error;
  return p + d;
}

/**
 * @brief Establece el estado de la carrera y, en caso de haber comenzado, guarda el tiempo (ms) de inicio.
 *
 * @param started Indica si la carrera ha comenzado.
 */
void set_race_started(bool started) {
  race_started = started;
  if (started) {
    race_started_ms = millis();
  }else{
    race_stopped_ms = millis();
  }
}

/**
 * @brief Comprueba si la carrera está en curso.
 *
 * @return true En carrera.
 * @return false En espera.
 */
bool is_race_started() {
  return race_started;
}

/**
 * @brief Obtiene los ms de inicio de la carrera.
 * Comparando este dato con el resultado de millis() se puede calcular el tiempo transcurrido desde el inicio de la carrera.
 *
 * @return long Tiempo de inicio de la carrera en ms.
 */
long get_race_started_ms() {
  return race_started_ms;
}

long get_race_stopped_ms() {
  return race_stopped_ms;
}

/**
 * @brief Establece la velocidad base del robot.
 *
 * @param speed Velocidad base.
 */
void set_base_speed(int speed) {
  base_speed = speed;
}

/**
 * @brief Establece la aceleración inicial del robot.
 *
 * @param accel_speed Aceleración inicial.
 */
void set_base_accel_speed(int accel_speed) {
  base_accel_speed = accel_speed;
}

/**
 * @brief Bucle de control principal. Realiza el cálculo de la corrección del controlador PID y establece la velocidad de los motores y el ventilador.
 * Esta función debe llamarse lo más frecuentemente posible. El tiempo entre ejecuciones está definido por la constante CONTROL_LOOP_US.
 * 
 */
void control_loop() {
  if (micros() - last_control_loop_us > CONTROL_LOOP_US || micros() < last_control_loop_us) {
    position = get_sensor_position(position);
    int correction = calc_correction(position);
    if (millis() - get_last_line_detected_ms() > 250) {
      set_motors_speed(0, 0);
      set_race_started(false);
    } else {

      if (speed < base_speed) {
        speed = base_accel_speed * (race_started_ms / 1000.0f);
      } else if (speed > base_speed) {
        speed = base_speed;
      }

      set_motors_speed(speed + correction, speed - correction);
    }
    last_control_loop_us = micros();
  }
}