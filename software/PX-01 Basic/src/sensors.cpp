#include <sensors.h>

static int sensors_raw[SENSORS_COUNT];
static long sensors_refresh_mc = 0;

static int sensors_max[SENSORS_COUNT] = {SENSORS_MIN, SENSORS_MIN, SENSORS_MIN, SENSORS_MIN, SENSORS_MIN, SENSORS_MIN, SENSORS_MIN, SENSORS_MIN, SENSORS_MIN, SENSORS_MIN, SENSORS_MIN, SENSORS_MIN, SENSORS_MIN, SENSORS_MIN, SENSORS_MIN, SENSORS_MIN};
static int sensors_min[SENSORS_COUNT] = {SENSORS_MAX, SENSORS_MAX, SENSORS_MAX, SENSORS_MAX, SENSORS_MAX, SENSORS_MAX, SENSORS_MAX, SENSORS_MAX, SENSORS_MAX, SENSORS_MAX, SENSORS_MAX, SENSORS_MAX, SENSORS_MAX, SENSORS_MAX, SENSORS_MAX, SENSORS_MAX};
static int sensors_umb[SENSORS_COUNT] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

static long last_line_detected_ms = 0;

/**
 * @brief Actualiza los valores de los sensores cada 1ms.
 * Se van leyendo los valores de dos en dos por cada estado de los multiplexores.
 * También se invierte el valor de los sensores en caso de haber seleccionado LINE_WHITE en la configuración.
 *
 */
static void refresh_sensors() {
  if (micros() - sensors_refresh_mc >= 1000 || micros() < sensors_refresh_mc) {

#ifdef LINE_BLACK
    sensors_raw[7] = analogRead(SENSOR_1);
    sensors_raw[8] = analogRead(SENSOR_2);
    digitalWrite(MUX_A, HIGH); // 0 0 1
    sensors_raw[6] = analogRead(SENSOR_1);
    sensors_raw[9] = analogRead(SENSOR_2);
    digitalWrite(MUX_B, HIGH); // 0 1 1
    sensors_raw[4] = analogRead(SENSOR_1);
    sensors_raw[11] = analogRead(SENSOR_2);
    digitalWrite(MUX_A, LOW); // 0 1 0
    sensors_raw[5] = analogRead(SENSOR_1);
    sensors_raw[10] = analogRead(SENSOR_2);
    digitalWrite(MUX_C, HIGH); // 1 1 0
    sensors_raw[1] = analogRead(SENSOR_1);
    sensors_raw[14] = analogRead(SENSOR_2);
    digitalWrite(MUX_A, HIGH); // 1 1 1
    sensors_raw[0] = analogRead(SENSOR_1);
    sensors_raw[15] = analogRead(SENSOR_2);
    digitalWrite(MUX_B, LOW); // 1 0 1
    sensors_raw[2] = analogRead(SENSOR_1);
    sensors_raw[13] = analogRead(SENSOR_2);
    digitalWrite(MUX_A, LOW); // 1 0 0
    sensors_raw[3] = analogRead(SENSOR_1);
    sensors_raw[12] = analogRead(SENSOR_2);
    digitalWrite(MUX_C, LOW); // 0 0 0
#else
    sensors_raw[7] = SENSORS_MAX - analogRead(SENSOR_1);
    sensors_raw[8] = SENSORS_MAX - analogRead(SENSOR_2);
    digitalWrite(MUX_A, HIGH); // 0 0 1
    sensors_raw[6] = SENSORS_MAX - analogRead(SENSOR_1);
    sensors_raw[9] = SENSORS_MAX - analogRead(SENSOR_2);
    digitalWrite(MUX_B, HIGH); // 0 1 1
    sensors_raw[4] = SENSORS_MAX - analogRead(SENSOR_1);
    sensors_raw[11] = SENSORS_MAX - analogRead(SENSOR_2);
    digitalWrite(MUX_A, LOW); // 0 1 0
    sensors_raw[5] = SENSORS_MAX - analogRead(SENSOR_1);
    sensors_raw[10] = SENSORS_MAX - analogRead(SENSOR_2);
    digitalWrite(MUX_C, HIGH); // 1 1 0
    sensors_raw[1] = SENSORS_MAX - analogRead(SENSOR_1);
    sensors_raw[14] = SENSORS_MAX - analogRead(SENSOR_2);
    digitalWrite(MUX_A, HIGH); // 1 1 1
    sensors_raw[0] = SENSORS_MAX - analogRead(SENSOR_1);
    sensors_raw[15] = SENSORS_MAX - analogRead(SENSOR_2);
    digitalWrite(MUX_B, LOW); // 1 0 1
    sensors_raw[2] = SENSORS_MAX - analogRead(SENSOR_1);
    sensors_raw[13] = SENSORS_MAX - analogRead(SENSOR_2);
    digitalWrite(MUX_A, LOW); // 1 0 0
    sensors_raw[3] = SENSORS_MAX - analogRead(SENSOR_1);
    sensors_raw[12] = SENSORS_MAX - analogRead(SENSOR_2);
    digitalWrite(MUX_C, LOW); // 0 0 0
#endif

    sensors_refresh_mc = micros();
  }
}

/**
 * @brief Imprime los valores de calibración de los sensores
 *
 */
static void print_calibrations() {
  Serial.println("Calibrations: ");
  for (int sensor = 0; sensor < SENSORS_COUNT; sensor++) {
    Serial.print(sensors_min[sensor]);
    Serial.print(" ");
    Serial.print(sensors_max[sensor]);
    Serial.print(" ");
    Serial.println(sensors_umb[sensor]);
  }
}

/**
 * @brief Calibra los sensores obteniendo los valores máximos y mínimos para calcular el umbral a partir del cual se considera que un sensor está detectando la línea.
 * El umbral se calcula como el 2/3 del rango de valores entre el máximo y el mínimo para saturar más la lectura y filtrar posibles imperfecciones de la pista.
 *
 */
void calibrate_sensors() {
  set_led(RGB_LEFT, 80, 20, 0);
  set_led(RGB_TOP, 80, 20, 0);
  set_led(RGB_RIGHT, 80, 20, 0);
  while (get_btn_pressed_state() != BTN_PRESSED) {
    blink_led(RGB_TOP, 80, 20, 0, 350);
  }
  clear_led(RGB_LEFT);
  clear_led(RGB_TOP);
  clear_led(RGB_RIGHT);
  int calibration_start_ms = millis();
  int count_ok = 0;
  do {
    rainbow_led(RGB_TOP);
    count_ok = 0;
    for (int sensor = 0; sensor < SENSORS_COUNT; sensor++) {
      int sensor_value = get_sensor_raw(sensor);
      if (sensor_value > sensors_max[sensor]) {
        sensors_max[sensor] = sensor_value;
      }
      if (sensor_value < sensors_min[sensor]) {
        sensors_min[sensor] = sensor_value;
      }
      sensors_umb[sensor] = sensors_min[sensor] + ((sensors_max[sensor] - sensors_min[sensor]) * 2 / 3);
      if (abs(sensors_max[sensor] - sensors_min[sensor]) >= 1000) {
        count_ok++;
      }
    }
  } while (millis() - calibration_start_ms < SENSORS_CALIBRATION_MS);
  print_calibrations();

  clear_led(RGB_LEFT);
  clear_led(RGB_TOP);
  clear_led(RGB_RIGHT);
  if (count_ok == SENSORS_COUNT) {
    set_led(RGB_TOP, 0, 50, 0);
    delay(1000);
  } else {
    while (get_btn_pressed_state() != BTN_LONG_PRESSED) {
      blink_led(RGB_TOP, 50, 0, 0, 75);
    }
  }
  clear_led(RGB_TOP);
  delay(500);
}

/**
 * @brief Obtiene el valor sin procesar de un sensor
 *
 * @param sensor Sensor a leer
 * @return int Valor sin procesar del sensor
 */
int get_sensor_raw(int sensor) {
  if (sensor >= 0 && sensor < SENSORS_COUNT) {
    refresh_sensors();
    return sensors_raw[sensor];
  } else {
    return -1;
  }
}

/**
 * @brief Obtiene el valor calibrado de un sensor
 *
 * @param sensor Sensor a leer
 * @return int Valor calibrado del sensor
 */
int get_sensor_calibrated(int sensor) {
  if (sensor >= 0 && sensor < SENSORS_COUNT) {
    refresh_sensors();
    return sensors_raw[sensor] >= sensors_umb[sensor] ? SENSORS_MAX : SENSORS_MIN;
  } else {
    return -1;
  }
}

/**
 * @brief Obtiene la posición del robot en la pista
 *
 * @param last_position Última posición conocida del robot
 * @return int Posición del robot en la pista
 */
int get_sensor_position(int last_position) {
  long sum_sensors_weight = 0;
  long sum_sensors = 0;
  int count_sensors_detecting = 0;
  for (int sensor = 0; sensor < SENSORS_COUNT; sensor++) {
    int sensor_value = get_sensor_calibrated(sensor);
    if (sensor_value >= sensors_umb[sensor]) {
      count_sensors_detecting++;
    }
    sum_sensors_weight += (sensor + 1) * sensor_value * 1000;
    sum_sensors += sensor_value;
  }

  int position_max = ((1000 * (SENSORS_COUNT + 1)) / 2);
  int position = 0;
  if (count_sensors_detecting > 0 && count_sensors_detecting < SENSORS_COUNT) {
    position = (sum_sensors_weight / sum_sensors) - position_max;
    last_line_detected_ms = millis();
  } else {
    position = last_position >= 0 ? position_max : -position_max;
  }
  return map(position, -position_max, position_max, -SENSORS_POSITION_MAX, SENSORS_POSITION_MAX);
}

/**
 * @brief Obtiene el tiempo en ms desde la última vez que se detectó la línea.
 * Comparando este dato con el resultado de millis() se puede calcular el tiempo transcurrido desde la última vez que se detectó la línea.
 * De esta forma se puede detener el robot de forma segura en caso de pérdida de línea.
 *
 * @return long Tiempo en ms de la última detección de línea.
 */
long get_last_line_detected_ms() {
  return last_line_detected_ms;
}
