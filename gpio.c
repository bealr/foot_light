#include "gpio.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define GPIO_CHIP "/dev/gpiochip0"  // Utilisez le nom du chip, pas le chemin /dev/

struct gpio_t *gpio_init() {

    pthread_t th;

    struct gpio_t *gpio_st = (struct gpio_t *) malloc(sizeof(struct gpio_t));
    int i;

    uint8_t array_gpio[5] = {5, 6, 13, 19, 26};

    struct gpiod_chip *chip;
    struct gpiod_line_request *request;
    struct gpiod_line_settings *settings;
    struct gpiod_request_config *req_cfg;
    struct gpiod_line_config *line_cfg;
    unsigned int offsets[1];  // Un seul offset à la fois
    int ret;

    // Ouvrir le chip GPIO une seule fois
    chip = gpiod_chip_open(GPIO_CHIP);
    if (!chip) {
        fprintf(stderr, "Erreur: impossible d'ouvrir le chip GPIO %s\n", GPIO_CHIP);
        free(gpio_st);
        return NULL;
    }

    // Stocker le chip dans la structure
    gpio_st->chip = chip;

    for (i = 0; i < 5; i++) {
        offsets[0] = array_gpio[i];  // Correction: utiliser l'index [0], pas [1]

        // Créer les paramètres de ligne
        settings = gpiod_line_settings_new();
        if (!settings) {
            fprintf(stderr, "Erreur: impossible de créer les paramètres de ligne\n");
            // Nettoyer les ressources précédentes
            for (int j = 0; j < i; j++) {
                gpiod_line_request_release(gpio_st->requests[j]);
            }
            gpiod_chip_close(chip);
            free(gpio_st);
            return NULL;
        }

        // Configurer la ligne en entrée
        gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_INPUT);
        gpiod_line_settings_set_bias(settings, GPIOD_LINE_BIAS_PULL_UP); // Pull-up interne

        // Créer la configuration de ligne
        line_cfg = gpiod_line_config_new();
        if (!line_cfg) {
            fprintf(stderr, "Erreur: impossible de créer la configuration de ligne\n");
            gpiod_line_settings_free(settings);
            // Nettoyer les ressources précédentes
            for (int j = 0; j < i; j++) {
                gpiod_line_request_release(gpio_st->requests[j]);
            }
            gpiod_chip_close(chip);
            free(gpio_st);
            return NULL;
        }

        // Appliquer les paramètres à la ligne
        ret = gpiod_line_config_add_line_settings(line_cfg, offsets, 1, settings);
        if (ret) {
            fprintf(stderr, "Erreur: impossible d'ajouter les paramètres à la ligne GPIO %d\n", array_gpio[i]);
            gpiod_line_config_free(line_cfg);
            gpiod_line_settings_free(settings);
            // Nettoyer les ressources précédentes
            for (int j = 0; j < i; j++) {
                gpiod_line_request_release(gpio_st->requests[j]);
            }
            gpiod_chip_close(chip);
            free(gpio_st);
            return NULL;
        }

        // Créer la configuration de requête
        req_cfg = gpiod_request_config_new();
        if (!req_cfg) {
            fprintf(stderr, "Erreur: impossible de créer la configuration de requête\n");
            gpiod_line_config_free(line_cfg);
            gpiod_line_settings_free(settings);
            // Nettoyer les ressources précédentes
            for (int j = 0; j < i; j++) {
                gpiod_line_request_release(gpio_st->requests[j]);
            }
            gpiod_chip_close(chip);
            free(gpio_st);
            return NULL;
        }

        gpiod_request_config_set_consumer(req_cfg, "gpio-reader");

        // Faire la requête de ligne
        request = gpiod_chip_request_lines(chip, req_cfg, line_cfg);
        if (!request) {
            fprintf(stderr, "Erreur: impossible de faire la requête de ligne GPIO %d\n", array_gpio[i]);
            gpiod_request_config_free(req_cfg);
            gpiod_line_config_free(line_cfg);
            gpiod_line_settings_free(settings);
            // Nettoyer les ressources précédentes
            for (int j = 0; j < i; j++) {
                gpiod_line_request_release(gpio_st->requests[j]);
            }
            gpiod_chip_close(chip);
            free(gpio_st);
            return NULL;
        }

        // Stocker la requête dans la structure selon le pin
        switch (array_gpio[i]) {
            case 5:
                gpio_st->line_5 = request;
                break;
            case 6:
                gpio_st->line_6 = request;
                break;
            case 13:
                gpio_st->line_13 = request;
                break;
            case 19:
                gpio_st->line_19 = request;
                break;
            case 26:
                gpio_st->line_26 = request;
                break;
        }

        // Stocker aussi dans le tableau pour le nettoyage
        gpio_st->requests[i] = request;

        // Nettoyer les ressources temporaires
        gpiod_request_config_free(req_cfg);
        gpiod_line_config_free(line_cfg);
        gpiod_line_settings_free(settings);

        printf("GPIO %d initialisée avec succès\n", array_gpio[i]);
    }

    pthread_create(&th, NULL, gpio_machine_th, gpio_st);

    return gpio_st;
}

int gpio_get(struct gpio_t *gpio_st, enum e_gpio_switch gpio) {
    uint8_t pin;
    struct gpiod_line_request *request;
    enum gpiod_line_value value;

    if (!gpio_st) {
        fprintf(stderr, "Erreur: gpio_st est NULL\n");
        return -1;
    }

    switch (gpio) {
        case SWITCH_SPEED_LOW:
            pin = 5;
            request = gpio_st->line_5;
            break;
        case SWITCH_SPEED_MED:
            pin = 6;
            request = gpio_st->line_6;
            break;
        case SWITCH_SPEED_HIGH:
            pin = 13;
            request = gpio_st->line_13;
            break;
        case SWITCH_SPEED_FLASH:
            pin = 19;
            request = gpio_st->line_19;
            break;
        case SWITCH_SPEED_FREEZE:
            pin = 26;
            request = gpio_st->line_26;
            break;
        default:
            fprintf(stderr, "Erreur: GPIO switch invalide\n");
            return -1;
    }

    if (!request) {
        fprintf(stderr, "Erreur: request pour GPIO %d est NULL\n", pin);
        return -1;
    }

    value = gpiod_line_request_get_value(request, pin);
    if (value < 0) {
        fprintf(stderr, "Erreur: impossible de lire la valeur GPIO %d\n", pin);
        return -1;
    }

    return (value == GPIOD_LINE_VALUE_ACTIVE) ? 0 : 1;
}

void gpio_cleanup(struct gpio_t *gpio_st) {
    if (!gpio_st) return;

    // Libérer toutes les requêtes
    for (int i = 0; i < 5; i++) {
        if (gpio_st->requests[i]) {
            gpiod_line_request_release(gpio_st->requests[i]);
        }
    }

    // Fermer le chip
    if (gpio_st->chip) {
        gpiod_chip_close(gpio_st->chip);
    }

    // Libérer la structure
    free(gpio_st);
}

void *gpio_machine_th(void *arg) {

    struct gpio_t *gpio_st = (struct gpio_t *) arg;
    uint8_t x;

    int i;

    while (1) {

        usleep(10000);

        gpio_st->real_state[SWITCH_SPEED_LOW] = gpio_get(gpio_st, SWITCH_SPEED_LOW);
        gpio_st->real_state[SWITCH_SPEED_MED] = gpio_get(gpio_st, SWITCH_SPEED_MED);
        gpio_st->real_state[SWITCH_SPEED_HIGH] = gpio_get(gpio_st, SWITCH_SPEED_HIGH);
        gpio_st->real_state[SWITCH_SPEED_FLASH] = gpio_get(gpio_st, SWITCH_SPEED_FLASH);
        gpio_st->real_state[SWITCH_SPEED_FREEZE] = gpio_get(gpio_st, SWITCH_SPEED_FREEZE);

        if (gpio_st->real_state[SWITCH_SPEED_FLASH] && gpio_st->real_state[SWITCH_SPEED_FREEZE]) {
            gpio_st->current_state[SWITCH_SPEED_BLACKOUT] = SWITCH_PRESS_TYPE_LONG;
            gpio_st->current_state[SWITCH_SPEED_FLASH] = 0;
            gpio_st->current_state[SWITCH_SPEED_FREEZE] = 0;
            continue;
        }

        for (i=0; i<3; i++) {

            if (gpio_st->real_state[i] == 0)
                gpio_st->switch_accumulate[i] = 0;
            else if (gpio_st->switch_accumulate[i] < 254)
                gpio_st->switch_accumulate[i]++;

            if (gpio_st->switch_accumulate[i] > 25 && gpio_st->current_state[i] != SWITCH_PRESS_TYPE_LONG_ACK)
                gpio_st->current_state[i] = SWITCH_PRESS_TYPE_LONG;

            if (gpio_st->real_state[i] == 1 && gpio_st->real_state_save[i] == 0)
                gpio_st->current_state[i] = SWITCH_PRESS_TYPE_SHORT;
        }

        if (gpio_st->real_state[SWITCH_SPEED_FLASH] == 1 && gpio_st->current_state[SWITCH_SPEED_FLASH] != 2)
            gpio_st->current_state[SWITCH_SPEED_FLASH] = 3;
        else if (gpio_st->real_state[SWITCH_SPEED_FLASH] == 0 && gpio_st->real_state_save[SWITCH_SPEED_FLASH] == 1)
            gpio_st->current_state[SWITCH_SPEED_FLASH] = 1;

        if (gpio_st->real_state[SWITCH_SPEED_FREEZE] == 1 && gpio_st->current_state[SWITCH_SPEED_FREEZE] != 2)
            gpio_st->current_state[SWITCH_SPEED_FREEZE] = 3;
        else if (gpio_st->real_state[SWITCH_SPEED_FREEZE] == 0 && gpio_st->real_state_save[SWITCH_SPEED_FREEZE] == 1)
            gpio_st->current_state[SWITCH_SPEED_FREEZE] = 1;

        for (i=0; i<5; i++)
            gpio_st->real_state_save[i] = gpio_st->real_state[i];

        x = 0;
        for (i=0; i<5; i++)
            x += gpio_st->real_state[i];

        if (x)
            usleep(100000);

    }

    return NULL;
}