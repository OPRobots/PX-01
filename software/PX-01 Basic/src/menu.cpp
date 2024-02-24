#include <menu.h>

static int8_t menu_index = 0;

/**
 * @brief En función de la selección del menú, establece la velocidad de motores y ventilador y la aceleración del robot.
 * Además, muestra el LED RGB correspondiente.
 *
 */
static void handle_menu_index() {
  switch (menu_index) {
    case 0:
      set_led(RGB_LEFT, 0, 0, 50);
      clear_led(RGB_TOP);
      clear_led(RGB_RIGHT);
      set_base_speed(MENU_1_BASE_SPEED);
      set_base_accel_speed(MENU_1_ACCEL_SPEED);
      break;
    case 1:
      clear_led(RGB_LEFT);
      set_led(RGB_TOP, 0, 0, 50);
      clear_led(RGB_RIGHT);
      set_base_speed(MENU_2_BASE_SPEED);
      set_base_accel_speed(MENU_2_ACCEL_SPEED);
      break;
    case 2:
      clear_led(RGB_LEFT);
      clear_led(RGB_TOP);
      set_led(RGB_RIGHT, 0, 0, 50);
      set_base_speed(MENU_3_BASE_SPEED);
      set_base_accel_speed(MENU_3_ACCEL_SPEED);
      break;
  }
}

/**
 * @brief Gestiona el menú de selección de velocidades.
 *
 * @param btn_state Estado del botón.
 */
void handle_menu(BTN_STATES btn_state) {
  if (btn_state == BTN_PRESSED) {
    menu_index = (menu_index + 1) % 3;
  }
  handle_menu_index();
}