#ifndef GPIO_H
#define GPIO_H

#include <gpiod.h>
#include <stdint.h>
#include <pthread.h>

enum e_gpio_switch {
    SWITCH_SPEED_LOW,
    SWITCH_SPEED_MED,
    SWITCH_SPEED_HIGH,
    SWITCH_SPEED_FLASH,
    SWITCH_SPEED_FREEZE,
    SWITCH_SPEED_BLACKOUT
};

enum e_switch_press_type {
    SWITCH_PRESS_TYPE_SHORT_ACK = 0,
    SWITCH_PRESS_TYPE_SHORT,
    SWITCH_PRESS_TYPE_LONG_ACK,
    SWITCH_PRESS_TYPE_LONG
};

struct gpio_t {
    struct gpiod_chip *chip;
    struct gpiod_line_request *line_5;
    struct gpiod_line_request *line_6;
    struct gpiod_line_request *line_13;
    struct gpiod_line_request *line_19;
    struct gpiod_line_request *line_26;
    struct gpiod_line_request *requests[5];  // Pour le nettoyage
    uint8_t current_state[6];
    uint8_t real_state[6];
    uint8_t real_state_save[6];
    uint8_t switch_accumulate[6];
};

// Fonctions publiques
struct gpio_t *gpio_init(void);
int gpio_get(struct gpio_t *gpio_st, enum e_gpio_switch gpio);
void gpio_cleanup(struct gpio_t *gpio_st);
void *gpio_machine_th(void *arg);

#endif // GPIO_H