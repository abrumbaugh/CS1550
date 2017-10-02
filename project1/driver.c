/*
    Alec Brumbaugh, arb178
    CS 1550 - Project 1: Double-Buffered Graphics Driver Progam
*/

#include "graphics.h"

int main(int argc, char **argv)
{
    char key;
    void *buffer;

    init_graphics();

    buffer = new_offscreen_buffer();

    // Draws "HI"
    draw_line(buffer, 25, 25, 25, 75, RGB(0, 63, 31));
    draw_line(buffer, 75, 25, 75, 75, RGB(0, 63, 31));
    draw_line(buffer, 25, 50, 75, 50, RGB(0, 63, 31));
    draw_line(buffer, 85, 25, 135, 25, RGB(0, 63, 31));
    draw_line(buffer, 110, 25, 110, 75, RGB(0, 63, 31));
    draw_line(buffer, 85, 75, 135, 75, RGB(0, 63, 31));
    blit(buffer);

    do
    {
        key = getkey();

        if(key == 'q')
        {
            break;
        }
        else if(key == '+')
        {
            clear_screen(buffer);

            // Draws 'A'
            draw_line(buffer, 50, 50, 50, 100, RGB(0, 63, 31));
            draw_line(buffer, 50, 50, 100, 50, RGB(0, 63, 31));
            draw_line(buffer, 100, 50, 100, 100, RGB(0, 63, 31));
            draw_line(buffer, 50, 70, 100, 70, RGB(0, 63, 31));

            // Draws 'L'
            draw_line(buffer, 110, 50, 110, 100, RGB(0, 63, 31));
            draw_line(buffer, 110, 100, 160, 100, RGB(0, 63, 31));

            // Draws 'E'
            draw_line(buffer, 170, 50, 170, 100, RGB(0, 63, 31));
            draw_line(buffer, 170, 50, 220, 50, RGB(0, 63, 31));
            draw_line(buffer, 170, 75, 220, 75, RGB(0, 63, 31));
            draw_line(buffer, 170, 100, 220, 100, RGB(0, 63, 31));

            // Draws 'C'
            draw_line(buffer, 230, 50, 230, 100, RGB(0, 63, 31));
            draw_line(buffer, 230, 50, 280, 50, RGB(0, 63, 31));
            draw_line(buffer, 230, 100, 280, 100, RGB(0, 63, 31));

            blit(buffer);
        }
        sleep_ms(200);
    }
    while(1);

    exit_graphics();
    return 0;
}
