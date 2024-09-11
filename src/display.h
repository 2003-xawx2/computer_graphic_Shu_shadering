#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include "utility.h"

#define FIXED_FPS 60
#define FRAME_TARGET_TIME (1000 / FIXED_FPS)

extern int window_width;
extern int window_height;
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern uint32_t *color_buffer; // unsigned 32bit-long int to represent color in ARGB
extern SDL_Texture *color_buffer_texture;

bool initialize_window(void);
void destory_window();
void set_up_color_buffer();
void render_color_buffer();
void draw_pixel(int x, int y, uint32_t color);
void clear_color_buffer(uint32_t color);
void draw_grid(int w, int h, uint32_t color);
void draw_rect(int pos_x, int pos_y, int w, int h, uint32_t color);
void draw_line(int x0, int y0, int x1, int y1, uint32_t color);
void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, int32_t color);
void draw_textured_triangle(int x0, int y0, float u0, float v0,
                            int x1, int y1, float u1, float v1,
                            int x2, int y2, float u2, float v2,uint32_t* texture);

#endif