#include "color.h"
#include <stdint.h>

color_t uint32_to_color(uint32_t color)
{
    color_t result;

    // 提取 alpha, red, green, blue 分量
    result.r = (color >> 24) & 0xFF; // 提取 alpha
    result.g = (color >> 16) & 0xFF; // 提取 red
    result.b = (color >> 8) & 0xFF;  // 提取 green
    result.a = color & 0xFF;         // 提取 blue

    // 将分量转换为 0.0 到 1.0 的范围，并确保不超过 1 或小于 0
    result.r = (result.r / 255.0f);
    result.g = (result.g / 255.0f);
    result.b = (result.b / 255.0f);
    result.a = (result.a / 255.0f);

    // 限制范围
    result.a = result.a < 0.0f ? 0.0f : (result.a > 1.0f ? 1.0f : result.a);
    result.r = result.r < 0.0f ? 0.0f : (result.r > 1.0f ? 1.0f : result.r);
    result.g = result.g < 0.0f ? 0.0f : (result.g > 1.0f ? 1.0f : result.g);
    result.b = result.b < 0.0f ? 0.0f : (result.b > 1.0f ? 1.0f : result.b);

    return result;
}

uint32_t color_to_uint32(color_t color)
{
    uint32_t result = 0;

    // 将分量限制在 0 到 1 的范围内
    color.a = color.a < 0.0f ? 0.0f : (color.a > 1.0f ? 1.0f : color.a);
    color.r = color.r < 0.0f ? 0.0f : (color.r > 1.0f ? 1.0f : color.r);
    color.g = color.g < 0.0f ? 0.0f : (color.g > 1.0f ? 1.0f : color.g);
    color.b = color.b < 0.0f ? 0.0f : (color.b > 1.0f ? 1.0f : color.b);

    // 将分量转换为 0 到 255 的范围并合并为 uint32_t
    uint8_t alpha = (uint8_t)(color.a * 255.0f);
    uint8_t red = (uint8_t)(color.r * 255.0f);
    uint8_t green = (uint8_t)(color.g * 255.0f);
    uint8_t blue = (uint8_t)(color.b * 255.0f);

    result |= (red << 24);   // 设置 alpha
    result |= (green << 16); // 设置 red
    result |= (blue << 8);   // 设置 green
    result |= alpha;         // 设置 blue

    return result;
}