#include <Arduino.h>

#include <config.h>
#include <control.h>
#include <menu.h>
#include <pinout.h>
#include <sensors.h>
#include <utils.h>

bool race_started = false;
long race_started_ms = 0;

void setup() {
  init_components();
  calibrate_sensors();
}

void loop() {

  if (!is_race_started()) {
    BTN_STATES btn_state = get_btn_pressed_state();
    if (btn_state == BTN_PRESSING && get_btn_pressing_ms() >= 250) {
      clear_led(RGB_LEFT);
      clear_led(RGB_TOP);
      clear_led(RGB_RIGHT);
      while (btn_state == BTN_PRESSING) {
        blink_led(RGB_RIGHT, 0, 0, 50, 125);
        blink_led(RGB_TOP, 0, 0, 50, 125);
        blink_led(RGB_LEFT, 0, 0, 50, 125);
        btn_state = get_btn_pressed_state();
      }
    } else {
      handle_menu(btn_state);
    }

    if (btn_state == BTN_LONG_PRESSED) {
      set_led(RGB_LEFT, 0, 0, 50);
      set_led(RGB_TOP, 0, 0, 50);
      set_led(RGB_RIGHT, 0, 0, 50);
      long starting_ms = millis();
      while (millis() < (starting_ms + get_ms_start())) {
        if (get_ms_start() > 3000 && millis() - starting_ms < get_ms_start() - 3000) {
          blink_led(RGB_RIGHT, 0, 0, 50, 500);
          blink_led(RGB_TOP, 0, 0, 50, 500);
          blink_led(RGB_LEFT, 0, 0, 50, 500);
        } else {
          if (millis() - starting_ms < get_ms_start() - 2000) {
            set_led(RGB_LEFT, 0, 0, 50);
            set_led(RGB_TOP, 0, 0, 50);
            set_led(RGB_RIGHT, 0, 0, 50);
          } else if (millis() - starting_ms < get_ms_start() - 1000) {
            set_led(RGB_RIGHT, 0, 0, 50);
            set_led(RGB_LEFT, 0, 0, 50);
            clear_led(RGB_TOP);
          } else {
            set_led(RGB_TOP, 0, 0, 50);
            clear_led(RGB_LEFT);
            clear_led(RGB_RIGHT);
          }
        }
      }
      clear_led(RGB_LEFT);
      clear_led(RGB_TOP);
      clear_led(RGB_RIGHT);
      set_race_started(true);
    }
  } else {
    control_loop();
    if ((millis() - get_race_started_ms()) >= TEST_DURATION_MS) {
      set_motors_speed(0, 0);
      set_race_started(false);
    }
  }
}
