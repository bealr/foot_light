#include <stdio.h>
#include "dmx.h"
#include "starville_4_bundle.h"

int main (int argc, char *argv[]) {

    struct dmx_data_t *dmx_data;
    struct starville_4_bundle_t *starville_4_bundle;

    srand(time(NULL));

    dmx_data = dmx_init();
    dmx_data->fd = dmx_open("/dev/ttyUSB0");
    dmx_run(dmx_data);

    starville_4_bundle = starville_4_bundle_init(dmx_data, 0);

    starville_4_bundle_set_dimmer(starville_4_bundle, 255);

    int i;

    starville_4_bundle_rand_mask(starville_4_bundle);
    starville_4_bundle_rand_color(starville_4_bundle);
    starville_4_bundle_rot(starville_4_bundle);

    while (1) {

        sleep(2);

        dmx_data->channels[2] = 255;

        sleep(2);


        dmx_data->channels[2] = 0;
        


    }
}


/*
    slow prg

    starville_4_bundle_rand_mask(starville_4_bundle);
    starville_4_bundle_rand_color(starville_4_bundle);

    for (i=0;i<500;i++) {
        starville_4_bundle_rot(starville_4_bundle);
        usleep(10000);
    }


    fast prg

    starville_4_bundle_rand_mask(starville_4_bundle);

        for (i=0;i<10;i++) {
        starville_4_bundle_rand_color(starville_4_bundle);
        starville_4_bundle_rot(starville_4_bundle);
        usleep(100000);
    }

    flash

    dmx_data->channels[2] = 255;

*/