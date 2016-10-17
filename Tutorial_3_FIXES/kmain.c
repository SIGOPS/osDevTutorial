#include "terminal.h"
#include "serial.h"

void kmain(void) {
	terminal_write("TEST\n", 5);
	// Loop nicely
	asm volatile(".2: hlt; jmp .2;");

}


