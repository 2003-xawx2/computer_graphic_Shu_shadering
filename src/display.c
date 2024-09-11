#include "display.h"

int window_width = 800;
int window_height = 600;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
uint32_t *color_buffer = NULL; // unsigned 32bit-long int to represent color in ARGB
SDL_Texture *color_buffer_texture = NULL;

bool initialize_window(void)
{
    // Init SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    { // there are errors occurred when init
        fprintf(stderr, "ERROR: initializing window!\n");
        return false;
    }

    // Create a SDL Window
    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);
    window_width = display_mode.w;
    window_height = display_mode.h;

    window = SDL_CreateWindow(
        "3d_renderer",                                  // title
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, // pos x and y
        window_width, window_height,                    // width and height
        SDL_WINDOW_METAL | SDL_WINDOW_FULLSCREEN_DESKTOP);

    if (!window)
    {
        fprintf(stderr, "ERROR: creating window!\n");
        return false;
    }

    // Create a SDL Renderer
    renderer = SDL_CreateRenderer(
        window,
        -1, // just get the first default one
        0 /*no need to pass ant flag*/);

    if (!renderer)
    {
        fprintf(stderr, "ERROR: creating renderer!\n");
        return false;
    }

    return true;
}

void destory_window()
{
    free(color_buffer);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

void set_up_color_buffer()
{
    color_buffer = (uint32_t *)malloc(sizeof(uint32_t) * window_width * window_height);
    color_buffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,    // color encode format
        SDL_TEXTUREACCESS_STREAMING, // texture accsess whwn update
        window_width, window_height);
}

void render_color_buffer()
{ // copy the color buffer to the texture and tell renderer to render it
    SDL_UpdateTexture(
        color_buffer_texture,
        NULL, // the rect need to update
        color_buffer,
        sizeof(uint32_t) * window_width);
    SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void draw_pixel(int x, int y, uint32_t color)
{
    if (x >= window_width || y >= window_height || x < 0 || y < 0)
    {
        return;
    }

    color_buffer[y * window_width + x] = color;
}

void clear_color_buffer(uint32_t color)
{
    for (int y = 0; y < window_height; y++)
        for (int x = 0; x < window_width; x++)
            draw_pixel(x, y, color);
}

void draw_grid(int w, int h, uint32_t color)
{
    for (int y = 0; y < window_height; y += h)
    {
        for (int x = 0; x < window_width; x += w)
        {
            draw_pixel(x, y, color);
        }
    }
}

void draw_rect(int pos_x, int pos_y, int w, int h, uint32_t color)
{
    for (int y = pos_y; y < pos_y + h; y++)
    {
        for (int x = pos_x; x < pos_x + w; x++)
        {
            draw_pixel(x, y, color);
        }
    }
}

void draw_line(int x0, int y0, int x1, int y1, uint32_t color)
{
    int delta_x = x1 - x0;
    int delta_y = y1 - y0;

    int side_length = abs(delta_x) > abs(delta_y) ? abs(delta_x) : abs(delta_y);

    float x_inc = delta_x / (float)side_length;
    float y_inc = delta_y / (float)side_length;

    float current_x = x0;
    float current_y = y0;

    for (int i = 0; i <= side_length; i++)
    {
        draw_pixel(round(current_x), round(current_y), color);
        current_x += x_inc;
        current_y += y_inc;
    }
}

void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, int32_t color)
{
    // sort vertices by y => y2>=y1>=y0
    if (y0 > y1)
    {
        int_swap(&x0, &x1);
        int_swap(&y0, &y1);
    }
    if (y1 > y2)
    {
        int_swap(&x1, &x2);
        int_swap(&y1, &y2);
    }
    if (y0 > y1)
    {
        int_swap(&x0, &x1);
        int_swap(&y0, &y1);
    }

    // divide the triangle by the y1
    /*
        (x2,y2)
        |\
        | \
        |  \
 (x1,y1)|___\ (xm,ym)
         \_  \
           \_ \
             \_\ (x0,y0)
     */

    int ym = y1;
    int xm = (float)(x2 - x0) * (y1 - y0) / (float)(y2 - y0) + x0;

    // draw bottom flat triangle
    float x1_inc = y2 - y1 == 0 ? 0 : (x1 - x2) / (float)abs(y2 - y1);
    float xm_inc = y2 - ym == 0 ? 0 : (xm - x2) / (float)abs(y2 - ym);

    float current_x1 = x2;
    float current_xm = x2;

    for (int y = y2; y >= y1; y--)
    {
        if (xm > x1)
        {
            for (int x = current_x1; x <= current_xm; x++)
                draw_pixel(x, y, color);
        }
        else
        {
            for (int x = current_xm; x <= current_x1; x++)
                draw_pixel(x, y, color);
        }

        current_x1 += x1_inc;
        current_xm += xm_inc;
    }

    // draw top flat triangle
    x1_inc = y0 - y1 == 0 ? 0 : (x1 - x0) / (float)abs(y0 - y1);
    xm_inc = y0 - ym == 0 ? 0 : (xm - x0) / (float)abs(y0 - ym);

    current_x1 = x0;
    current_xm = x0;

    for (int y = y0; y <= y1; y++)
    {
        if (xm > x1)
        {
            for (int x = current_x1; x <= current_xm; x++)
                draw_pixel(x, y, color);
        }
        else
        {
            for (int x = current_xm; x <= current_x1; x++)
                draw_pixel(x, y, color);
        }
        current_x1 += x1_inc;
        current_xm += xm_inc;
    }
}

void draw_textured_triangle(int x0, int y0, float u0, float v0,
                            int x1, int y1, float u1, float v1,
                            int x2, int y2, float u2, float v2, uint32_t *texture)
{
    // sort vertices by y => y2>=y1>=y0
    if (y0 > y1)
    {
        int_swap(&x0, &x1);
        int_swap(&y0, &y1);
        int_swap(&u0, &u1);
        int_swap(&v0, &v1);
    }
    if (y1 > y2)
    {
        int_swap(&x1, &x2);
        int_swap(&y1, &y2);
        int_swap(&u1, &u2);
        int_swap(&v1, &v2);
    }
    if (y0 > y1)
    {
        int_swap(&x0, &x1);
        int_swap(&y0, &y1);
        int_swap(&u0, &u1);
        int_swap(&v0, &v1);
    }

    int ym = y1;
    int xm = (float)(x2 - x0) * (y1 - y0) / (float)(y2 - y0) + x0;

    // draw bottom flat triangle
    float x1_inc = y2 - y1 == 0 ? 0 : (x1 - x2) / (float)abs(y2 - y1);
    float xm_inc = y2 - ym == 0 ? 0 : (xm - x2) / (float)abs(y2 - ym);

    float current_x1 = x2;
    float current_xm = x2;

    for (int y = y2; y >= y1; y--)
    {
        // if (xm > x1)
        // {
        //     for (int x = current_x1; x <= current_xm; x++)
        //         draw_pixel(x, y, color);
        // }
        // else
        // {
        //     for (int x = current_xm; x <= current_x1; x++)
        //         draw_pixel(x, y, color);
        // }

        current_x1 += x1_inc;
        current_xm += xm_inc;
    }

    // draw top flat triangle
    x1_inc = y0 - y1 == 0 ? 0 : (x1 - x0) / (float)abs(y0 - y1);
    xm_inc = y0 - ym == 0 ? 0 : (xm - x0) / (float)abs(y0 - ym);

    current_x1 = x0;
    current_xm = x0;

    for (int y = y0; y <= y1; y++)
    {
        // if (xm > x1)
        // {
        //     for (int x = current_x1; x <= current_xm; x++)
        //         draw_pixel(x, y, color);
        // }
        // else
        // {
        //     for (int x = current_xm; x <= current_x1; x++)
        //         draw_pixel(x, y, color);
        // }
        current_x1 += x1_inc;
        current_xm += xm_inc;
    }
}