/*
    Alec Brumbaugh, arb178
    CS 1550 - Project 1: Double-Buffered Graphics Header File
*/

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <termios.h>
#include <linux/fb.h>
#include <unistd.h>

// color type
typedef unsigned short color_t;

// RGB conversion
#define RGB(r, g, b) ((color_t) ((r << 11) | (g << 5) | b))

// function prototypes
void init_graphics();
void exit_graphics();
char getkey();
void sleep_ms(long ms);
void clear_screen(void *img);
void draw_pixel(void *img, int x, int y, color_t color);
void draw_line(void *img, int x1, int y1, int x2, int y2, color_t c);
void *new_offscreen_buffer();
void blit(void *src);
void fix();

#endif
