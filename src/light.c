#include "light.h"
#include "color.h"

uint32_t shade_direct_light_color(uint32_t color, vec3_t normal, direct_light_t light)
{

    float percent = -(vec3_dot(vec3_normalize(normal), vec3_normalize(light.direction)) - 1) / 2.0;

    return apply_lght_intensity(color, percent);
}

uint32_t apply_lght_intensity(uint32_t color, float intensity_factor)
{
    color_t c = uint32_to_color(color);
    c.r *= intensity_factor;
    c.g *= intensity_factor;
    c.b *= intensity_factor;
    return color_to_uint32(c);
}