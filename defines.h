#ifndef __DEFINES_H
#define __DEFINES_H

#include <stdint.h>

#define MODE_NO 0
#define MODE_LOW 1
#define MODE_MED 2
#define MODE_HIGH 3
#define MODE_FLASH 4
#define MODE_FREEZE 5
#define MODE_BLACKOUT 6

struct current_mode_t {

    uint8_t current_mode;
    uint8_t current_mode_save;
};

#endif