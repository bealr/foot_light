#include "ludipocket5g.h"

struct ludipocket_5g_t *ludipocket_5g_init(struct dmx_data_t *dmx_data, uint16_t channel) {

    struct ludipocket_5g_t *ludipocket_5g = (struct ludipocket_5g_t *) malloc(sizeof(struct ludipocket_5g_t));

    ludipocket_5g->channel = channel -1;
    ludipocket_5g->dmx_data = dmx_data;

    ludipocket_5g_motor_cmd(ludipocket_5g, 0, 0);
    ludipocket_5g_motor_cmd(ludipocket_5g, 1, 0);

    ludipocket_5g->dmx_data->channels[ludipocket_5g->channel + 2] = 255;

    return ludipocket_5g;
}

void ludipocket_5g_motor_cmd(struct ludipocket_5g_t *ludipocket_5g, uint8_t mot_id, uint8_t angle) {

    pthread_mutex_lock(&ludipocket_5g->dmx_data->channel_mutex);
    
    if (mot_id == 0) {
        ludipocket_5g->dmx_data->channels[ludipocket_5g->channel + 0] = angle;
        ludipocket_5g->mem_mot[0] = angle;
    }

    if (mot_id == 1) {
        ludipocket_5g->dmx_data->channels[ludipocket_5g->channel + 1] = angle;
        ludipocket_5g->mem_mot[1] = angle;
    }

    pthread_mutex_unlock(&ludipocket_5g->dmx_data->channel_mutex);
}

void ludipocket_5g_motor_mot_rand(struct ludipocket_5g_t *ludipocket_5g) {

    uint8_t mot_1 = rand() * 255;
    uint8_t mot_2 = rand() * 255;

    ludipocket_5g_motor_cmd(ludipocket_5g, 0, mot_1);
    ludipocket_5g_motor_cmd(ludipocket_5g, 1, mot_2);
}

void ludipocket_5g_motor_incr(struct ludipocket_5g_t *ludipocket_5g) {


    if (ludipocket_5g->mem_mot[0] > 254)
        ludipocket_5g->mem_rot_dir[0] = 1;
    else if (ludipocket_5g->mem_mot[0] < 1)
        ludipocket_5g->mem_rot_dir[0] = 0;

    if (ludipocket_5g->mem_mot[1] > 254)
        ludipocket_5g->mem_rot_dir[1] = 1;
    else if (ludipocket_5g->mem_mot[1] < 1)
        ludipocket_5g->mem_rot_dir[1] = 0;

    if (ludipocket_5g->mem_rot_dir[0])
        ludipocket_5g_motor_cmd(ludipocket_5g, 0, --ludipocket_5g->mem_mot[0]);
    else
        ludipocket_5g_motor_cmd(ludipocket_5g, 0, ++ludipocket_5g->mem_mot[0]);

    if (ludipocket_5g->mem_rot_dir[1])
        ludipocket_5g_motor_cmd(ludipocket_5g, 1, --ludipocket_5g->mem_mot[1]);
    else
        ludipocket_5g_motor_cmd(ludipocket_5g, 1, ++ludipocket_5g->mem_mot[1]);
}

void ludipocket_5g_set_leds(struct ludipocket_5g_t *ludipocket_5g, uint8_t side, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {

    pthread_mutex_lock(&ludipocket_5g->dmx_data->channel_mutex);
    
    if (side == 0) {
        ludipocket_5g->dmx_data->channels[ludipocket_5g->channel + 4] = r;
        ludipocket_5g->dmx_data->channels[ludipocket_5g->channel + 5] = g;
        ludipocket_5g->dmx_data->channels[ludipocket_5g->channel + 6] = b;
        ludipocket_5g->dmx_data->channels[ludipocket_5g->channel + 7] = w;
    }
    if (side == 1) {
        ludipocket_5g->dmx_data->channels[ludipocket_5g->channel + 8] = r;
        ludipocket_5g->dmx_data->channels[ludipocket_5g->channel + 9] = g;
        ludipocket_5g->dmx_data->channels[ludipocket_5g->channel + 10] = b;
        ludipocket_5g->dmx_data->channels[ludipocket_5g->channel + 11] = w;
    }

    pthread_mutex_unlock(&ludipocket_5g->dmx_data->channel_mutex);

}

void ludipocket_5g_set_dimmer(struct ludipocket_5g_t *ludipocket_5g, uint8_t dimmer) {

    pthread_mutex_lock(&ludipocket_5g->dmx_data->channel_mutex);
    
    ludipocket_5g->dmx_data->channels[ludipocket_5g->channel + 2] = dimmer;

    pthread_mutex_unlock(&ludipocket_5g->dmx_data->channel_mutex);
}

void ludipocket_5g_set_flash(struct ludipocket_5g_t *ludipocket_5g, uint8_t flash) {

    uint8_t f = flash;

    // if (flash > 244)
    //     flash = 244;

    // flash = 254 - flash;
    pthread_mutex_lock(&ludipocket_5g->dmx_data->channel_mutex);
    
    ludipocket_5g->dmx_data->channels[ludipocket_5g->channel + 3] = flash;

    printf("srtobe : %d\n", flash);

    pthread_mutex_unlock(&ludipocket_5g->dmx_data->channel_mutex);
}