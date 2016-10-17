#include "terminal.h"
#include "serial.h"

void kmain(void) {

	// Move value into ESI for testing
	asm volatile("movl $0xFFEEDD, %esi");


	// Print some text!
	fb_write_cell(0, 'S', COL_WHITE, COL_BLACK);
	fb_write_cell(2, 'I', COL_WHITE, COL_BLACK);
	fb_write_cell(4, 'G', COL_WHITE, COL_BLACK);
	fb_write_cell(6, 'O', COL_WHITE, COL_BLACK);
	fb_write_cell(8, 'P', COL_WHITE, COL_BLACK);
	fb_write_cell(10, 'S', COL_WHITE, COL_BLK);

	// Loop nicely
	asm volatile(".1: hlt; jmp .1;");

}


