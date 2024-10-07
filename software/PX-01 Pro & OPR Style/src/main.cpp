#include <Arduino.h>

#include "config.h"
#include "control.h"
#include "menu.h"
#include "pinout.h"
#include "rc5.h"
#include "sensors.h"
#include "utils.h"

bool race_started = false;
long race_started_ms = 0;
bool state = false;

void setup() {
  init_components();
  calibrate_sensors();
}

void loop() {
  // rc5_update();
  // printf("%d\n", is_race_starting());
  // delay(200);

  // set_led(RGB_RIGHT, 0, 0, 50);
  // clear_led(RGB_LEFT);
  // clear_led(RGB_TOP);
  // return;

  if (!is_race_started() && !is_race_starting()) {
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
      if (btn_state == BTN_LONG_PRESSED) {
        set_race_starting(true);
      }
    } else {
      handle_menu(btn_state);
    }
    set_fan_speed(0);
    set_motors_speed(0, 0);
  } else if (is_race_starting()) {
    set_led(RGB_LEFT, 0, 0, 50);
    set_led(RGB_TOP, 0, 0, 50);
    set_led(RGB_RIGHT, 0, 0, 50);
    long starting_ms = millis();
    while (millis() < (starting_ms + get_ms_start()) && is_race_starting()) {
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
          if (get_base_fan_speed() != 0) {
            set_fan_speed(get_base_fan_speed());
          }
          control_loop();
        }
      }
    }
    if (is_race_starting()) {
      clear_led(RGB_LEFT);
      clear_led(RGB_TOP);
      clear_led(RGB_RIGHT);
      set_race_started(true);
    }
  } else {
    control_loop();
    // set_fan_speed(15);
    // if ((millis() - get_race_started_ms()) >= TEST_DURATION_MS) {
    //   set_fan_speed(0);
    //   set_motors_speed(0, 0);
    //   set_race_started(false);
    // }
  }
}
