#include "starville_4_bundle.h"

struct starville_4_bundle_t *starville_4_bundle_init(struct dmx_data_t *dmx_data, uint16_t channel) {

    struct starville_4_bundle_t *starville_4_bundle = (struct starville_4_bundle_t *) malloc(sizeof(struct starville_4_bundle_t));

    if (starville_4_bundle) {
        starville_4_bundle->channel = channel -1;
        starville_4_bundle->dmx_data = dmx_data;
        memset(starville_4_bundle->mem_rot, 0, 12);
        memset(starville_4_bundle->mem_rot_mask, 0, 12);
        memset(starville_4_bundle->mem_rot_dir, 0, 12);

        return starville_4_bundle;
    }

    return NULL;
}

void starville_4_bundle_set_dimmer(struct starville_4_bundle_t *starville_4_bundle, uint8_t dimmer) {

    pthread_mutex_lock(&starville_4_bundle->dmx_data->channel_mutex);

    starville_4_bundle->dmx_data->channels[starville_4_bundle->channel + 1] = dimmer;

    pthread_mutex_unlock(&starville_4_bundle->dmx_data->channel_mutex);
}

void starville_4_bundle_set_flash(struct starville_4_bundle_t *starville_4_bundle, uint8_t flash) {

    pthread_mutex_lock(&starville_4_bundle->dmx_data->channel_mutex);

    starville_4_bundle->dmx_data->channels[2] = flash;

    pthread_mutex_unlock(&starville_4_bundle->dmx_data->channel_mutex);
}


void starville_4_bundle_set_leds_spot(struct starville_4_bundle_t *starville_4_bundle, uint8_t spot, uint8_t r, uint8_t g, uint8_t b) {

    pthread_mutex_lock(&starville_4_bundle->dmx_data->channel_mutex);

    starville_4_bundle->dmx_data->channels[starville_4_bundle->channel + spot*3 + 3] = r;
    starville_4_bundle->dmx_data->channels[starville_4_bundle->channel + spot*3 + 4] = g;
    starville_4_bundle->dmx_data->channels[starville_4_bundle->channel + spot*3 + 5] = b;

    pthread_mutex_unlock(&starville_4_bundle->dmx_data->channel_mutex);
}

void starville_4_bundle_rot(struct starville_4_bundle_t *starville_4_bundle) {

    int i;

    for (i=0; i< 12; i++) {
        if (starville_4_bundle->mem_rot_mask[i]) {

            if (starville_4_bundle->mem_rot[i] < 20)
                starville_4_bundle->mem_rot_dir[i] = 0;

            if (starville_4_bundle->mem_rot[i] == 255)
                starville_4_bundle->mem_rot_dir[i] = 1;

            if (starville_4_bundle->mem_rot_dir[i])
                starville_4_bundle->mem_rot[i]--;
            else
                starville_4_bundle->mem_rot[i]++;
        }
    }

    for (i=0; i<4; i++)
        starville_4_bundle_set_leds_spot(starville_4_bundle, i, starville_4_bundle->mem_rot[i*3 +0], starville_4_bundle->mem_rot[i*3 +1] ,starville_4_bundle->mem_rot[i*3 +2]);
}

void starville_4_bundle_rand_color(struct starville_4_bundle_t *starville_4_bundle) {
    
    int i, res;
    uint8_t rgb[3];
    uint8_t dir;

    for (i=0; i<4; i++) {

        do {
                rgb[0] = rand() * 255;
                rgb[1] = rand() * 255;
                rgb[2] = rand() * 255;

                res = rgb[0] + rgb[1] + rgb[2];

        } while (res < 256);


            starville_4_bundle->mem_rot_dir[i*3 +0] = (rand() % 2) * 1;
            starville_4_bundle->mem_rot_dir[i*3 +1] = (rand() % 2) * 1;
            starville_4_bundle->mem_rot_dir[i*3 +2] = (rand() % 2) * 1;

            if (starville_4_bundle->mem_rot_mask[i*3 +0])
                starville_4_bundle->mem_rot[i*3 +0] = rgb[0];
            else
                starville_4_bundle->mem_rot[i*3 +0] = 0;

            if (starville_4_bundle->mem_rot_mask[i*3 +1])
                starville_4_bundle->mem_rot[i*3 +1] = rgb[1];
            else
                starville_4_bundle->mem_rot[i*3 +1] = 0;

            if (starville_4_bundle->mem_rot_mask[i*3 +2])
                starville_4_bundle->mem_rot[i*3 +2] = rgb[2];
            else
                starville_4_bundle->mem_rot[i*3 +2] = 0;

    }
}

void starville_4_bundle_rand_mask(struct starville_4_bundle_t *starville_4_bundle) {

    int i, res, do_it_again;

    int mask[3] = {0}; // R G B

    do {

        do_it_again = 0;

        mask[0] = (rand() % 2) * 1;
        mask[1] = (rand() % 2) * 1;
        mask[2] = (rand() % 2) * 1;

        res = mask[0] + mask[1] + mask[2];

        if (starville_4_bundle->mem_rot_mask[0] == mask[0] && starville_4_bundle->mem_rot_mask[1] == mask[1] && starville_4_bundle->mem_rot_mask[2] == mask[2]) {
            do_it_again = 1;
            printf("doing rand mask again !\n");
        }

    } while (res < 1 || do_it_again);

    if (res == 1) {

        for (i=0; i<2; i++) {
            starville_4_bundle->mem_rot_mask[i*3 +0] = mask[0];
            starville_4_bundle->mem_rot_mask[i*3 +1] = mask[1];
            starville_4_bundle->mem_rot_mask[i*3 +2] = mask[2];
        }
        for (i=2; i<4; i++) {
            starville_4_bundle->mem_rot_mask[i*3 +0] = mask[2];
            starville_4_bundle->mem_rot_mask[i*3 +1] = mask[0];
            starville_4_bundle->mem_rot_mask[i*3 +2] = mask[1];
        }
    }
    else {
        for (i=0; i<4; i++) {
            starville_4_bundle->mem_rot_mask[i*3 +0] = mask[0];
            starville_4_bundle->mem_rot_mask[i*3 +1] = mask[1];
            starville_4_bundle->mem_rot_mask[i*3 +2] = mask[2];
        }
    }
}