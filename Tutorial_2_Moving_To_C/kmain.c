#define COL_RED     0
#define COL_BLK 	4

char *fb = (char *) 0x000B8000;

/** fb_write_cell:
 *  Writes a character with the given foreground and background to position i
 *  in the framebuffer.
 *
 *  @param i  The location in the framebuffer
 *  @param c  The character
 *  @param fg The foreground color
 *  @param bg The background color
 */
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
    fb[i] = c;
    fb[i + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}

void kmain(void) {

	// Move value into ESI for testing
	asm volatile("movl $0xFFEEDD, %esi");


	// Print some text!
	fb_write_cell(0, 'S', COL_RED, COL_BLK);
	fb_write_cell(2, 'I', COL_RED, COL_BLK);
	fb_write_cell(4, 'G', COL_RED, COL_BLK);
	fb_write_cell(6, 'O', COL_RED, COL_BLK);
	fb_write_cell(8, 'P', COL_RED, COL_BLK);
	fb_write_cell(10, 'S', COL_RED, COL_BLK);

	// Loop nicely
	asm volatile(".1: hlt; jmp .1;");

}


