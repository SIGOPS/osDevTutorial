#include "terminal.h"

uint8_t *fb = (uint8_t *) 0x000B8000;
uint32_t terminal_column = 0;
uint32_t terminal_row = 0;
uint8_t terminal_color = 0;

enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};


static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}

static inline uint16_t vga_entry(uint8_t uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}

void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	uint32_t y, x;
	for (y = 0; y < VGA_HEIGHT; y++) {
		for (x = 0; x < VGA_WIDTH; x++) {
			const uint32_t index = y * VGA_WIDTH + x;
			fb[index] = vga_entry(' ', terminal_color);
		}
	}
}


void terminal_putentryat(uint8_t c, uint8_t color, uint8_t x, uint8_t y) {
	const uint16_t index = y * VGA_WIDTH + x;
	fb[index] = vga_entry(c, color);
}

void terminal_putchar(uint8_t c) {
	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT) {
			terminal_row = 0;
		}
	} else {
		++terminal_column;
	}
}

void terminal_write(const char* data, uint32_t size) {
    uint32_t i;
    for (i = 0; i < size; i++) {
        terminal_putchar(data[i]);
    }
}
