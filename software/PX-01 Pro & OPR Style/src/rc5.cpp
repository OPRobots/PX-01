/**
    Basado en https://clearwater.com.au/code/rc5
 */
#include "rc5.h"
#include <Arduino.h>

enum RC5_TIMMINGS {
  MIN_SHORT = 444,
  MAX_SHORT = 1333,
  MIN_LONG = 1334,
  MAX_LONG = 2222
};

enum RC5_EVENT {
  EVENT_SHORTSPACE = 0,
  EVENT_SHORTPULSE = 2,
  EVENT_LONGSPACE = 4,
  EVENT_LONGPULSE = 6
};

enum RC5_STATE {
  STATE_START1 = 0,
  STATE_MID1 = 1,
  STATE_MID0 = 2,
  STATE_START0 = 3
};

enum RC5_MANAGE {
  S2_MASK = 0x1000,
  S2_SHIFT = 12,
  TOGGLE_MASK = 0x0800,
  TOGGLE_SHIFT = 11,
  ADDRESS_MASK = 0x7C0,
  ADDRESS_SHIFT = 6,
  COMMAND_MASK = 0x003F,
  COMMAND_SHIFT = 0
};

enum RC5_ADDRESS {
  ADDRESS_PROG = 0x0B,
  ADDRESS_COMP = 0x07
};

static uint16_t rc5_stored_data[RC5_DATA_LENGTH];
enum RC5_DATA_INDEX {
  DATA_STOP = 0,
  DATA_START = 1
};

static const uint8_t trans[] = {0x01, 0x91, 0x9B, 0xFB};

static bool pin_state = false;
static uint8_t state = STATE_MID1;
static uint16_t cmd = 1;
static uint8_t bits = 1;
static uint32_t last_us = 0;

static void rc5_manage_command(uint16_t message) {
  // unsigned char toggle = (message & TOGGLE_MASK) >> TOGGLE_SHIFT;
  unsigned char address = (message & ADDRESS_MASK) >> ADDRESS_SHIFT;
  unsigned char command = (message & COMMAND_MASK) >> COMMAND_SHIFT;
  switch (address) {
    case ADDRESS_PROG:
      rc5_stored_data[0] = command;
      rc5_stored_data[1] = command + 1;
      // EEPROM.writeUShort(EEPROM_ADDR_RC5_START, rc5_stored_data[DATA_START]);
      // EEPROM.writeUShort(EEPROM_ADDR_RC5_STOP, rc5_stored_data[DATA_STOP]);
      // EEPROM.commit();
      // set_led(RGB_RIGHT, 0, 50, 0);
      // delay(1000);
      // clear_led(RGB_RIGHT);
      break;
    case ADDRESS_COMP:
      if (command == rc5_stored_data[DATA_START]) {
        set_race_starting(true);
      } else if (command == rc5_stored_data[DATA_STOP]) {
        set_race_started(false);
      }
      break;
    default:
      if (command == 0x01) {
        set_race_starting(true);
      } else if (command == 0x02) {
        set_race_started(false);
      }
      break;
  }
}

static void reset(void) {
  state = STATE_MID1;
  cmd = 1;
  bits = 1;
  last_us = micros();
}

static void rc5_decode_event(enum RC5_EVENT event) {

  unsigned char newState = (trans[state] >> event) & 0x3;
  if (newState == state) {
    reset();
  } else {
    state = newState;
    if (newState == STATE_MID0) {
      cmd = (cmd << 1);
      bits++;
    } else if (newState == STATE_MID1) {
      cmd = (cmd << 1) + 1;
      bits++;
    }
  }
}

static void rc5_decode_pulse(enum RC5_TRIGGER trigger, uint32_t elapsed) {

  if (elapsed >= MIN_SHORT && elapsed <= MAX_SHORT) {
    rc5_decode_event(trigger == RC5_TRIGGER_FALLING ? EVENT_SHORTSPACE : EVENT_SHORTPULSE);
  } else if (elapsed >= MIN_LONG && elapsed <= MAX_LONG) {
    rc5_decode_event(trigger == RC5_TRIGGER_FALLING ? EVENT_LONGSPACE : EVENT_LONGPULSE);
  } else {
    reset();
  }
}

void rc5_init(void) {
  // rc5_stored_data[DATA_START] = EEPROM.readUShort(EEPROM_ADDR_RC5_START);
  // rc5_stored_data[DATA_STOP] = EEPROM.readUShort(EEPROM_ADDR_RC5_STOP);
}

void rc5_register(enum RC5_TRIGGER trigger) {
  uint32_t us = micros();
  rc5_decode_pulse(trigger, us - last_us);
  last_us = us;

  delayMicroseconds(100);

  if (bits == 14) {
    rc5_manage_command(cmd);
    cmd = 0;
    bits = 0;
  }
}

void rc5_isr(void) {
  rc5_register(digitalRead(MOD_START) ? RC5_TRIGGER_RISING : RC5_TRIGGER_FALLING);
}

void rc5_update(void) {
  bool new_pin_state = digitalRead(MOD_START);
  if (new_pin_state != pin_state) {
    pin_state = new_pin_state;
    rc5_register(pin_state ? RC5_TRIGGER_RISING : RC5_TRIGGER_FALLING);
  }
}