#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>

typedef struct
{
    float r, g, b, a;
} color_t;

color_t uint32_to_color(uint32_t color);
uint32_t color_to_uint32(color_t color);

#endif