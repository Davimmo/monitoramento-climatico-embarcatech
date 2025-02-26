#ifndef LEDS_H
#define LEDS_H

#include <stdint.h>
#include <stdbool.h>

#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define LED_RED     13
#define LED_GREEN   11
#define LED_BLUE    12

void led_init(uint8_t pin);
void led_intensity(uint8_t pin, uint8_t intensity);

#endif