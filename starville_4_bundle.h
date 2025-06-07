#ifndef __STARVILLE_4_BUNDLE_H
#define __STARVILLE_4_BUNDLE_H

#include "dmx.h"

struct starville_4_bundle_t {

    struct dmx_data_t *dmx_data;
    uint16_t channel;

    uint8_t mem_rot[12];
    uint8_t mem_rot_mask[12];
    uint8_t mem_rot_dir[12];
};

struct starville_4_bundle_t *starville_4_bundle_init(struct dmx_data_t *dmx_data, uint16_t channel);
void starville_4_bundle_set_dimmer(struct starville_4_bundle_t *starville_4_bundle, uint8_t dimmer);
void starville_4_bundle_set_flash(struct starville_4_bundle_t *starville_4_bundle, uint8_t flash);
void starville_4_bundle_set_leds_spot(struct starville_4_bundle_t *starville_4_bundle, uint8_t spot, uint8_t r, uint8_t g, uint8_t b);

void starville_4_bundle_rot(struct starville_4_bundle_t *starville_4_bundle);
void starville_4_bundle_rand_color(struct starville_4_bundle_t *starville_4_bundle);
void starville_4_bundle_rand_mask(struct starville_4_bundle_t *starville_4_bundle);

#endif