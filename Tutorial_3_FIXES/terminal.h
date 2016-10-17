#ifndef INCLUDE_TERMINAL_H
#define INCLUDE_TERMINAL_H

#include "io.h"
#include "types.h"

/* The I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

/* Colors possible for terminal */
#define COL_WHITE	15
#define COL_BLACK	0

/* Terminal Properties */
#define VGA_WIDTH	80
#define VGA_HEIGHT	25

void fb_move_cursor(uint16_t pos);
void fb_write_cell(uint32_t i, char c, uint8_t fg, uint8_t bg);
void terminal_initialize(void);
void terminal_write(const char* data, uint32_t size);

#endif
