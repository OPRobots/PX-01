#ifndef UTILS_H
#define UTILS_H

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <pinout.h>

enum LEDS {
  RGB_LEFT = 0,
  RGB_TOP = 1,
  RGB_RIGHT = 2
};

void set_led(enum LEDS led, int r, int g, int b);
void clear_led(enum LEDS led);

void blink_led(enum LEDS led, int r, int g, int b, int ms);
void rainbow_led(enum LEDS led);

/**
 * @brief Definición de los posibles estados del botón
 *
 */
enum BTN_STATES {
  BTN_RELEASED,
  BTN_PRESSING,
  BTN_PRESSED,
  BTN_LONG_PRESSED,
};

enum BTN_STATES get_btn_pressed_state();
long get_btn_pressing_ms();

#endif // UTILS_H