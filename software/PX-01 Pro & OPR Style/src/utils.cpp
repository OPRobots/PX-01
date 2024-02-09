#include <utils.h>

static Adafruit_NeoPixel pixels(7, NEOPIXEL, NEO_GRB + NEO_KHZ800);

static bool toggle_led_state[3] = {false, false, false};
static int blink_led_ms[3] = {0, 0, 0};

static int rainbow_led_ms[3] = {0, 0, 0};
int rainbow_led_RGB[3][3] = {{255, 0, 0}, {255, 0, 0}, {255, 0, 0}};
int rainbow_led_color_desc[3] = {0, 0, 0};
int rainbow_led_color_asc[3] = {1, 1, 1};

static int btn_pressed_ms = 0;
static int btn_last_pressed_ms = 0;

/**
 * @brief Activa un LED con un color determinado
 *
 * @param led LED a modificar
 * @param r Red
 * @param g Green
 * @param b Blue
 */
void set_led(enum LEDS led, int r, int g, int b) {
  pixels.setPixelColor(led, pixels.Color(r, g, b));
  pixels.show();
  toggle_led_state[led] = true;
  blink_led_ms[led] = millis();
}

/**
 * @brief Apaga un LED
 *
 * @param led LED a apagar
 */
void clear_led(enum LEDS led) {
  pixels.setPixelColor(led, pixels.Color(0, 0, 0));
  pixels.show();
  toggle_led_state[led] = false;
  blink_led_ms[led] = millis();
}

/**
 * @brief Parpadea un LED a una frecuencia determinada
 *
 * @param led LED a parpadear
 * @param ms Frecuencia de parpadeo
 */
void blink_led(enum LEDS led, int r, int g, int b, int ms) {
  if (millis() - blink_led_ms[led] >= ms) {
    if (toggle_led_state[led]) {
      clear_led(led);
    } else {
      set_led(led, r, g, b);
    }
  }
}

/**
 * @brief Aplica un efecto arcoiris a un LED RGB
 *
 * @param led
 * @param ms
 */
void rainbow_led(enum LEDS led) {
  if (millis() > rainbow_led_ms[led] + 30) {
    rainbow_led_ms[led] = millis();
    rainbow_led_RGB[led][rainbow_led_color_desc[led]] -= 20;
    rainbow_led_RGB[led][rainbow_led_color_asc[led]] += 20;
    pixels.setPixelColor(led, pixels.Color(rainbow_led_RGB[led][0], rainbow_led_RGB[led][1], rainbow_led_RGB[led][2]));
    pixels.show();
    if (rainbow_led_RGB[led][rainbow_led_color_desc[led]] <= 0 || rainbow_led_RGB[led][rainbow_led_color_asc[led]] >= 255) {
      rainbow_led_RGB[led][rainbow_led_color_desc[led]] = 0;
      rainbow_led_RGB[led][rainbow_led_color_asc[led]] = 255;
      pixels.setPixelColor(led, pixels.Color(rainbow_led_RGB[led][0], rainbow_led_RGB[led][1], rainbow_led_RGB[led][2]));
      pixels.show();
      rainbow_led_color_desc[led]++;
      if (rainbow_led_color_desc[led] > 2) {
        rainbow_led_color_desc[led] = 0;
      }
      rainbow_led_color_asc[led] = rainbow_led_color_desc[led] == 2 ? 0 : rainbow_led_color_desc[led] + 1;
    }
  }
}

/**
 * @brief Obtiene el estado del botón pulsado
 *
 * @return enum BTN_STATES
 */
enum BTN_STATES get_btn_pressed_state() {
  if (millis() - btn_last_pressed_ms < 50) {
    return BTN_RELEASED;
  }
  if (digitalRead(BTN_1)) {
    // Serial.print("RELEASE ");
    // Serial.println(btn_pressed_ms);
    if (btn_pressed_ms == 0) {
      return BTN_RELEASED;
    } else {
      if (millis() - btn_pressed_ms >= 250) {
        btn_pressed_ms = 0;
        btn_last_pressed_ms = millis();
        return BTN_LONG_PRESSED;
      } else if (millis() - btn_pressed_ms <= 250) {
        btn_pressed_ms = 0;
        if (millis() - btn_pressed_ms < 25) {
          return BTN_RELEASED;
        } else {
          btn_last_pressed_ms = millis();
          return BTN_PRESSED;
        }
      } else {
        return BTN_RELEASED;
      }
    }
  } else {
    // Serial.println("PRESSING");
    if (btn_pressed_ms == 0) {
      btn_pressed_ms = millis();
    }
    return BTN_PRESSING;
  }
}

/**
 * @brief Obtiene el tiempo que lleva pulsado el botón
 * 
 * @return long Tiempo en ms
 */
long get_btn_pressing_ms() {
  return millis() - btn_pressed_ms;
}