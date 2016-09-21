#ifndef INCLUDE_TERMINAL_H
#define INCLUDE_TERMINAL_H

#include "io.h"

/* The I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

/* Colors possible for terminal */
#define COL_WHITE	15
#define COL_BLACK	0

void fb_move_cursor(unsigned short pos);
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);
int terminal_write(char *buf, unsigned int len);

#endif
