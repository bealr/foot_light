#include <stdio.h>
#include "dmx.h"
#include "starville_4_bundle.h"
#include "gpio.h"
#include "defines.h"
#include "ludipocket5g.h"

int main (int argc, char *argv[]) {

    struct dmx_data_t *dmx_data;
    struct starville_4_bundle_t *starville_4_bundle;
    struct ludipocket_5g_t *ludipocket_5g;
    struct gpio_t *gpio;
    struct current_mode_t current_mode;
    uint8_t ludipocket_mot_cnt;
    int i;

    uint8_t cnt;

    current_mode.current_mode = MODE_LOW;
    current_mode.current_mode_save = MODE_NO;

    srand(time(NULL));

    gpio = gpio_init();
    dmx_data = dmx_init();
    dmx_data->fd = dmx_open("/dev/ttyUSB0");
    dmx_run(dmx_data);

    starville_4_bundle = starville_4_bundle_init(dmx_data, 1);
    ludipocket_5g = ludipocket_5g_init(dmx_data, 50);

    starville_4_bundle_set_dimmer(starville_4_bundle, 255);

    starville_4_bundle_rand_mask(starville_4_bundle);
    starville_4_bundle_rand_color(starville_4_bundle);
    starville_4_bundle_rot(starville_4_bundle);

    while (1) {


        if (gpio->current_state[SWITCH_SPEED_LOW] == SWITCH_PRESS_TYPE_SHORT) {
            
            current_mode.current_mode_save = current_mode.current_mode;
            current_mode.current_mode = MODE_LOW;
            starville_4_bundle_set_flash(starville_4_bundle, 0);
            ludipocket_5g_set_flash(ludipocket_5g, 0);
            starville_4_bundle_rand_mask(starville_4_bundle);
            starville_4_bundle_rand_color(starville_4_bundle);
            printf("MODE SLOW !!!!!!!! \n");

            gpio->current_state[SWITCH_SPEED_LOW] = SWITCH_PRESS_TYPE_SHORT_ACK;
        }
        if (gpio->current_state[SWITCH_SPEED_LOW] == SWITCH_PRESS_TYPE_LONG) {
            if (gpio->real_state[SWITCH_SPEED_LOW] == 0) {

                current_mode.current_mode = current_mode.current_mode_save;
                gpio->current_state[SWITCH_SPEED_LOW] = SWITCH_PRESS_TYPE_LONG_ACK;
            }
        }

        if (gpio->current_state[SWITCH_SPEED_MED] == SWITCH_PRESS_TYPE_SHORT) {
            
            current_mode.current_mode_save = current_mode.current_mode;
            current_mode.current_mode = MODE_MED;
            starville_4_bundle_set_flash(starville_4_bundle, 0);
            ludipocket_5g_set_flash(ludipocket_5g, 0);
            starville_4_bundle_rand_mask(starville_4_bundle);
            starville_4_bundle_rand_color(starville_4_bundle);
            printf("MODE MED !!!!!!!! \n");

            gpio->current_state[SWITCH_SPEED_MED] = SWITCH_PRESS_TYPE_SHORT_ACK;
        }
        if (gpio->current_state[SWITCH_SPEED_MED] == SWITCH_PRESS_TYPE_LONG) {
            if (gpio->real_state[SWITCH_SPEED_MED] == 0) {

                current_mode.current_mode = current_mode.current_mode_save;
                gpio->current_state[SWITCH_SPEED_MED] = SWITCH_PRESS_TYPE_LONG_ACK;
            }
        }

        if (gpio->current_state[SWITCH_SPEED_HIGH] == SWITCH_PRESS_TYPE_SHORT) {
            
            current_mode.current_mode_save = current_mode.current_mode;
            current_mode.current_mode = MODE_HIGH;
            starville_4_bundle_set_flash(starville_4_bundle, 0);
            ludipocket_5g_set_flash(ludipocket_5g, 0);
            starville_4_bundle_rand_mask(starville_4_bundle);
            printf("MODE HIGH !!!!!!!! \n");

            gpio->current_state[SWITCH_SPEED_HIGH] = SWITCH_PRESS_TYPE_SHORT_ACK;
        }
        if (gpio->current_state[SWITCH_SPEED_HIGH] == SWITCH_PRESS_TYPE_LONG) {
            if (gpio->real_state[SWITCH_SPEED_HIGH] == 0) {

                current_mode.current_mode = current_mode.current_mode_save;
                gpio->current_state[SWITCH_SPEED_HIGH] = SWITCH_PRESS_TYPE_LONG_ACK;
            }
        }

        if (gpio->current_state[SWITCH_SPEED_BLACKOUT] == SWITCH_PRESS_TYPE_LONG) {

            current_mode.current_mode = MODE_BLACKOUT;
            gpio->current_state[SWITCH_SPEED_BLACKOUT] = SWITCH_PRESS_TYPE_LONG_ACK;
            printf("BLACKOUT !!!!!!!! \n");
        }

        if (current_mode.current_mode != MODE_BLACKOUT) {

            if (gpio->current_state[SWITCH_SPEED_FLASH]) {

                if (gpio->current_state[SWITCH_SPEED_FLASH] == 3) {
                    current_mode.current_mode_save = current_mode.current_mode;
                    ludipocket_5g_set_flash(ludipocket_5g, 255);
                    starville_4_bundle_set_flash(starville_4_bundle, 255);
                    current_mode.current_mode = MODE_FLASH;
                    gpio->current_state[SWITCH_SPEED_FLASH] = 2;
                    printf("MODE STROB !!!!!!!! \n");
                }

                if (gpio->current_state[SWITCH_SPEED_FLASH] == 1) {
                    
                    current_mode.current_mode = current_mode.current_mode_save;
                    ludipocket_5g_set_flash(ludipocket_5g, 0);
                    starville_4_bundle_set_flash(starville_4_bundle, 0);
                    gpio->current_state[SWITCH_SPEED_FLASH] = 0;
                }
            }

            if (gpio->current_state[SWITCH_SPEED_FREEZE]) {

                if (gpio->current_state[SWITCH_SPEED_FREEZE] == 3) {
                    current_mode.current_mode_save = current_mode.current_mode;
                    current_mode.current_mode = MODE_FREEZE;
                    gpio->current_state[SWITCH_SPEED_FREEZE] = 2;
                    printf("MODE FREEZE !!!!!!!! \n");
                }

                if (gpio->current_state[SWITCH_SPEED_FREEZE] == 1) {
                    
                    current_mode.current_mode = current_mode.current_mode_save;
                    gpio->current_state[SWITCH_SPEED_FREEZE] = 0;
                }
            }
        }

        // ----------------

        if (current_mode.current_mode == MODE_LOW) {

            starville_4_bundle_rot(starville_4_bundle);
            ludipocket_5g_set_leds(ludipocket_5g, 1, starville_4_bundle->mem_rot[0], starville_4_bundle->mem_rot[1] ,starville_4_bundle->mem_rot[2], 0);
            ludipocket_5g_set_leds(ludipocket_5g, 0, starville_4_bundle->mem_rot[9], starville_4_bundle->mem_rot[10] ,starville_4_bundle->mem_rot[11], 0);
            ludipocket_5g_motor_incr(ludipocket_5g);
            usleep(30000);
        }

        if (current_mode.current_mode == MODE_MED) {

            starville_4_bundle_rot(starville_4_bundle);            
            ludipocket_5g_set_leds(ludipocket_5g, 1, starville_4_bundle->mem_rot[0], starville_4_bundle->mem_rot[1] ,starville_4_bundle->mem_rot[2], 0);
            ludipocket_5g_set_leds(ludipocket_5g, 0, starville_4_bundle->mem_rot[9], starville_4_bundle->mem_rot[10] ,starville_4_bundle->mem_rot[11], 0);
            ludipocket_5g_motor_incr(ludipocket_5g);
            usleep(5000);
        }

        if (current_mode.current_mode == MODE_HIGH) {

            starville_4_bundle_rand_color(starville_4_bundle);
            starville_4_bundle_rot(starville_4_bundle);
            ludipocket_5g_set_leds(ludipocket_5g, 1, starville_4_bundle->mem_rot[0], starville_4_bundle->mem_rot[1] ,starville_4_bundle->mem_rot[2], 0);
            ludipocket_5g_set_leds(ludipocket_5g, 0, starville_4_bundle->mem_rot[9], starville_4_bundle->mem_rot[10] ,starville_4_bundle->mem_rot[11], 0);
            if (ludipocket_mot_cnt++ > 3) {
                ludipocket_5g_motor_mot_rand(ludipocket_5g);
                ludipocket_mot_cnt = 0;
            }
            usleep(100000);
        }

        if (current_mode.current_mode == MODE_FLASH) {

            if (++cnt > 4)
                cnt = 0;


            if (cnt == 0)
                starville_4_bundle_set_leds_spot(starville_4_bundle, 0, 0, 0, 0);
            else
                starville_4_bundle_set_leds_spot(starville_4_bundle, 0, 255, 255, 255);

            if (cnt == 1)
                starville_4_bundle_set_leds_spot(starville_4_bundle, 1, 0, 0, 0);
            else
                starville_4_bundle_set_leds_spot(starville_4_bundle, 1, 255, 255, 255);
            
            if (cnt == 2)
                starville_4_bundle_set_leds_spot(starville_4_bundle, 2, 0, 0, 0);
            else
                starville_4_bundle_set_leds_spot(starville_4_bundle, 2, 255, 255, 255);
            
            if (cnt == 3)
                starville_4_bundle_set_leds_spot(starville_4_bundle, 3, 0, 0, 0);
            else
                starville_4_bundle_set_leds_spot(starville_4_bundle, 3, 255, 255, 255);

            ludipocket_5g_set_leds(ludipocket_5g, 0, 255, 255, 255, 255);
            ludipocket_5g_set_leds(ludipocket_5g, 1, 255, 255, 255, 255);
            usleep(100000);
        }

        if (current_mode.current_mode == MODE_FREEZE) {

            usleep(10000);
        }

        if (current_mode.current_mode == MODE_BLACKOUT) {

            starville_4_bundle_set_leds_spot(starville_4_bundle, 0, 0, 0, 0);
            starville_4_bundle_set_leds_spot(starville_4_bundle, 1, 0, 0, 0);
            starville_4_bundle_set_leds_spot(starville_4_bundle, 2, 0, 0, 0);
            starville_4_bundle_set_leds_spot(starville_4_bundle, 3, 0, 0, 0);

            ludipocket_5g_set_leds(ludipocket_5g, 0, 0, 0, 0, 0);
            ludipocket_5g_set_leds(ludipocket_5g, 1, 0, 0, 0, 0);

            gpio->current_state[SWITCH_SPEED_FLASH] = 0;
            gpio->current_state[SWITCH_SPEED_FREEZE] = 0;

            usleep(10000);
        }
    }
}
