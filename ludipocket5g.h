#ifndef __LUDIPOCKET_5G_H
#define __LUDIPOCKET_5G_H

#include "dmx.h"
#include <stdint.h>

struct ludipocket_5g_t {

    struct dmx_data_t *dmx_data;
    uint16_t channel;

    uint8_t mem_mot[2];
    uint8_t mem_dir[2];

    uint8_t mem_rot[12];
    uint8_t mem_rot_mask[12];
    uint8_t mem_rot_dir[12];
};

struct ludipocket_5g_t *ludipocket_5g_init(struct dmx_data_t *dmx_data, uint16_t channel);

void ludipocket_5g_motor_cmd(struct ludipocket_5g_t *ludipocket_5g, uint8_t mot_id, uint8_t angle);
void ludipocket_5g_motor_incr(struct ludipocket_5g_t *ludipocket_5g);

void ludipocket_5g_motor_mot_rand(struct ludipocket_5g_t *ludipocket_5g);

void ludipocket_5g_set_leds(struct ludipocket_5g_t *ludipocket_5g, uint8_t side, uint8_t r, uint8_t g, uint8_t b, uint8_t w);

void ludipocket_5g_set_dimmer(struct ludipocket_5g_t *ludipocket_5g, uint8_t dimmer);
void ludipocket_5g_set_flash(struct ludipocket_5g_t *ludipocket_5g, uint8_t flash);

#endif