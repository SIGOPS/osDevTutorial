#Step 1: Creating the Stack
All C programs use a stack, and since our OS is essentially a program, we must setup a stack for it as well. A trivial way to do this is to simply move the ESP register to a location. However, we cannot randomly move this register to a location that we like. Remember that the only thing in memory currently is the GRUB bootloader, BIOS, our OS and some I/O, so as to not conflict with any of these critical sections, we will reserve a piece of memory.

This is done in our `loader.s` with the following code:

``` asm
KERNEL_STACK_SIZE equ 4096                  ; size of stack in bytes

section .bss
align 4                                     ; align at 4 bytes
kernel_stack:                               ; label points to beginning of memory
    resb KERNEL_STACK_SIZE                  ; reserve stack for the kernel
```

After we have reserved this location in memory, we can simply move our `ESP` register to the newly allocated location:

``` asm 
mov esp, kernel_stack + KERNEL_STACK_SIZE   ; point esp to the start of the stack (end of memory area)
```


#Step 2: Calling C Code
Now that we have a stack, we can begin to move out of x86 and move into C code, dramatically increasing development speed. The first thing to do is to create a file caleld `kmain.c` which will contain all of our kernel code. Within `kmain.c` we can place this code:

```c
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
```

You may notice the `fb_write_cell` function. this exists to write text to the screen in a primitive fashion. 

#Step 3: Build Tools

The final step is to setup a Makefile for our OS so that we can easily run it. By using the following Makefile, we can simply call `make run` and compile and run our OS.

```Makefile
    OBJECTS = loader.o kmain.o
    CC = gcc
    CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
             -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c
    LDFLAGS = -T link.ld -melf_i386
    AS = nasm
    ASFLAGS = -f elf

    all: kernel.elf

    kernel.elf: $(OBJECTS)
        ld $(LDFLAGS) $(OBJECTS) -o kernel.elf

    os.iso: kernel.elf
        cp kernel.elf iso/boot/kernel.elf
        genisoimage -R                              \
                    -b boot/grub/stage2_eltorito    \
                    -no-emul-boot                   \
                    -boot-load-size 4               \
                    -A os                           \
                    -input-charset utf8             \
                    -quiet                          \
                    -boot-info-table                \
                    -o os.iso                       \
                    iso

    run: os.iso
        bochs -f bochsrc.txt -q

    %.o: %.c
        $(CC) $(CFLAGS)  $< -o $@

    %.o: %.s
        $(AS) $(ASFLAGS) $< -o $@

    clean:
        rm -rf *.o kernel.elf os.iso
```