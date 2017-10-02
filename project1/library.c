/*
    Alec Brumbaugh, arb178
    CS 1550 - Project 1: Double-Buffered Graphics Library
*/

#include "graphics.h"

void *framebuffer;      // Address that represents contents of the framebuffer file
int fb_descriptor;      // File descriptor for fb0
struct termios old_term_settings;        // Keeps track of old terminal settings
struct termios new_term_settings;        // New terminal settings
unsigned long x_virtual_axis;       // Variable that holds xres_virtual
unsigned long y_virtual_axis;       // Variable that holds yres_virtual
unsigned long virtual_line_len;     // Variable that holds line_length

void init_graphics()
{
    struct fb_var_screeninfo screeninfo_var;
    struct fb_fix_screeninfo screeninfo_fix;

    // Access fb0
    fb_descriptor = open("/dev/fb0", O_RDWR);

    // Error trying to open "/dev/fb0"
    if(fb_descriptor == -1)
    {
        _exit(1);
    }

    // Get the screen size and bits per pixel structs
    ioctl(fb_descriptor, FBIOGET_VSCREENINFO, &screeninfo_var);
    ioctl(fb_descriptor, FBIOGET_FSCREENINFO, &screeninfo_fix);

    // Assign the values to global variables
    y_virtual_axis = screeninfo_var.yres_virtual;
    x_virtual_axis = screeninfo_var.xres_virtual;
    virtual_line_len = screeninfo_fix.line_length;

    // Address that now represents the contents of the file
    framebuffer = mmap(NULL, (y_virtual_axis * virtual_line_len), PROT_READ | PROT_WRITE, MAP_SHARED, fb_descriptor, 0);

    // Error trying to map the memory
    if(framebuffer == (void *) -1)
    {
        _exit(1);
    }

    // Clear the screen
    write(STDIN_FILENO, "\033[2J", 4);

    // Get the old terminal settings, set ICANON and ECHO bits to 0, set the new terminal settings
    ioctl(STDIN_FILENO, TCGETS, &old_term_settings);
    new_term_settings = old_term_settings;
    new_term_settings.c_lflag &= ~(ICANON);
    new_term_settings.c_lflag &= ~(ECHO);
    ioctl(STDIN_FILENO, TCSETS, &new_term_settings);
}

void exit_graphics()
{
    // Set the terminal back to the old settings (re-enables ICANON and ECHO)
    ioctl(STDIN_FILENO, TCSETS, &old_term_settings);

    // Unmap the memory
    munmap(framebuffer, (y_virtual_axis * virtual_line_len));

    // Close the file
    close(fb_descriptor);
}

char getkey()
{
    char buf;       // The char that the user entered
    int key_pressed;       // Return value of the select() system call
    struct timeval timeout;
    fd_set read_fds;

    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);

    timeout.tv_sec = 10;    // Arbitrary timeout value
    timeout.tv_usec = 0;

    key_pressed = select(STDIN_FILENO + 1, &read_fds, NULL, NULL, &timeout);

    // The user pressed a key, get it and return the char
    if(key_pressed > 0)
    {
        if(FD_ISSET(STDIN_FILENO, &read_fds))
        {
            read(STDIN_FILENO, &buf, sizeof(buf));
            return buf;
        }
    }
}

void sleep_ms(long ms)
{
    struct timespec time_req;

    time_req.tv_sec = 0;
    time_req.tv_nsec = ms * 1000000L;

    // Sleep for the specified number of milliseconds
    nanosleep(&time_req, NULL);
}

void clear_screen(void *img)
{
    unsigned long i;

    // Clear the screen by looping through the image buffer parameter and
    // setting each byte to 0
    for(i = 0; i < (y_virtual_axis * virtual_line_len); i++)
    {
        ((char *)img)[i] = 0;
    }
}

void draw_pixel(void *img, int x, int y, color_t color)
{
    // Checks to make sure the coordinates are valid
    if(x < 0 || y < 0 || x > x_virtual_axis || y > y_virtual_axis)
    {
        return;
    }

    color_t *pixel_to_draw;
    int offset = (x * sizeof(unsigned short)) + (virtual_line_len * y);
    pixel_to_draw = img + offset;
    *pixel_to_draw = color;
}

void draw_line(void *img, int x1, int y1, int x2, int y2, color_t c)
{
    /*
    Draws a line using Bresenham's Algorithm
    Source: https://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C
    */

    int dx = abs(x2-x1), sx = x1<x2 ? 1 : -1;
    int dy = abs(y2-y1), sy = y1<y2 ? 1 : -1;
    int err = (dx>dy ? dx : -dy)/2, e2;

    for(;;)
    {
        draw_pixel(img,x1,y1,c);
        if (x1==x2 && y1==y2) break;
        e2 = err;
        if (e2 >-dx) { err -= dy; x1 += sx; }
        if (e2 < dy) { err += dx; y1 += sy; }
    }
}

void *new_offscreen_buffer()
{
    void *offscreen_buffer = mmap(NULL, (y_virtual_axis * virtual_line_len), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    // Error trying to create the offscreen buffer
    if (offscreen_buffer == (void *) -1)
    {
        _exit(1);
    }

    return offscreen_buffer;
}

void blit(void *src)
{
    unsigned long i;

    // Iterate through the source parameter and copy each byte to the framebuffer
    for(i = 0; i < (virtual_line_len * y_virtual_axis); i++)
    {
        ((char *)framebuffer)[i] = ((char *)src)[i];
    }
}

// Helper program to re-enable ICANON and ECHO if there is an abnormal termination
void fix()
{
    ioctl(STDIN_FILENO, TCSETS, &old_term_settings);
}
