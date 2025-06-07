#include "dmx.h"

int dmx_open(char *path) {
    
    struct termios2 tio;

    int fd = open(path, O_RDWR | O_NOCTTY);
    if (fd < 0) {
        perror("open");
        return -1;
    }

    if (ioctl(fd, TCGETS2, &tio) != 0) {
        perror("TCGETS2");
        return -1;
    }

    tio.c_cflag &= ~CBAUD;
    tio.c_cflag |= BOTHER;      // BOTHER = custom baudrate
    tio.c_ispeed = 250000;
    tio.c_ospeed = 250000;

    tio.c_cflag |= (CLOCAL | CREAD);
    tio.c_cflag &= ~PARENB;
    tio.c_cflag |= CSTOPB;      // 2 stop bits
    tio.c_cflag &= ~CSIZE;
    tio.c_cflag |= CS8;

    tio.c_iflag = IGNPAR;
    tio.c_oflag = 0;
    tio.c_lflag = 0;

    if (ioctl(fd, TCSETS2, &tio) != 0) {
        perror("TCSETS2");
        return -1;
    }

    return fd;
}

int dmx_send(struct dmx_data_t *dmx_data) {

    unsigned char frame[513];
    frame[0] = 0;

    if (dmx_data->fd <= 0 || !dmx_data) {

        printf("ERROR : check dmx_send\n");
        return -1;
    }

    

    pthread_mutex_lock(&dmx_data->channel_mutex);
    memcpy(&frame[1], dmx_data->channels, 512);
    pthread_mutex_unlock(&dmx_data->channel_mutex);

    ioctl(dmx_data->fd, TIOCSBRK);       // Force ligne TX à 0 (break)
    usleep(150);               // >88µs (100µs ici)
    ioctl(dmx_data->fd, TIOCCBRK);       // Fin du break
    usleep(20);                // Mark After Break (MAB >8µs)

    int ret = write(dmx_data->fd, frame, 513);

    if (ret < 0) {

        printf("ERROR : write to DMX\n");
        return -1;
    }

    if (ret == 513)
        return 0;

    return -1;
}

struct dmx_data_t *dmx_init() {

    struct dmx_data_t *dmx_data = (struct dmx_data_t *) malloc(sizeof(struct dmx_data_t));

    if (dmx_data) {

        memset(dmx_data->channels, 0, 512);

        pthread_mutex_init(&dmx_data->channel_mutex, NULL);
        
        return dmx_data;
    }

    return NULL;
}

void dmx_run(struct dmx_data_t *dmx_data) {

    pthread_t th;
    pthread_create(&th, NULL, dmx_send_th, dmx_data);
}

void *dmx_send_th(void *arg) {

    struct dmx_data_t *dmx_data = (struct dmx_data_t *) arg;
    int i;

    while (1) {

        dmx_send(dmx_data);

    // for (i=0; i<70;i++) {
    //     printf("%d ", dmx_data->channels[i]);
    //     if (i % 10 == 0) {
    //         printf("\n");
    //     }
    // }
    // printf("\n");
        
        usleep(25000);
    }

    return NULL;
}