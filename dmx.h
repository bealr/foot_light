#ifndef __DMX_H
#define __DMX_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/serial.h>
#include <asm/termbits.h>
#include <stdint.h>
#include <pthread.h>

struct dmx_data_t {
    int fd;
    uint8_t channels[512];
    pthread_mutex_t channel_mutex;
};


int dmx_open(char *path);
int dmx_send(struct dmx_data_t *dmx_data);
struct dmx_data_t *dmx_init();
void dmx_run(struct dmx_data_t *dmx_data);
void *dmx_send_th(void *arg);

#endif