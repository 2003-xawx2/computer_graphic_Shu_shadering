#ifndef LIGHT_H
#define LIGHT_H

#include "vector.h"
#include "stdint.h"

typedef struct
{
    vec3_t direction;
} direct_light_t;

uint32_t shade_direct_light_color(uint32_t color, vec3_t normal, direct_light_t light);
uint32_t apply_lght_intensity(uint32_t color, float intensity_factor);

#endif