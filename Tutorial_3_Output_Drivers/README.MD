#Step 1: Interacting with Hardware
As you may remember, last time we wrote a function that wrote text to the screen one character at a time. In this tutorial, let's change this so that all Display/Text functions are contained within their own file, so everthing is abstracted out.

The first step is Writing text to the screen. For this let's create a file called `terminal.c` and 'terminal.h'. Within this we will place all our functions relating to displaying text on the screen. Firstly we have the function we wrote last time `fb_write_cell`:

```C
/** fb_write_cell:
 *  Writes a character with the given foreground and background to position i
 *  in the framebuffer.
 *
 *  @param i  The location in the framebuffer
 *  @param c  The character
 *  @param fg The foreground color
 *  @param bg The background color
 */
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg) {
    fb[i] = c;
    fb[i + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F)
}
```
Let us also put some information in our `.h` file so that we can include this function elseware:

```C
#define FB_GREEN     2
#define FB_DARK_GREY 8

void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);
```

We would also like to move the blinking cursor along with our text, however this is not so easy to do.

#Step 2: IO Functions

In order to muve the cursor, we need to talk directly to VGA hardware. This is done through the use of `inb` and `outb` fuctions in x86, however, we would like to stay in C for as long as possible. Because of this, let's create a file called `io.s` and `io.h` and within these files place the `inb` and `outb` functions

### io.s
```asm
global outb             ; make the label outb visible outside this file
global inb

; outb - send a byte to an I/O port
; stack: [esp + 8] the data byte
;        [esp + 4] the I/O port
;        [esp    ] return address
outb:
    mov al, [esp + 8]    ; move the data to be sent into the al register
    mov dx, [esp + 4]    ; move the address of the I/O port into the dx register
    out dx, al           ; send the data to the I/O port
    ret                  ; return to the calling function

; inb - returns a byte from the given I/O port
; stack: [esp + 4] The address of the I/O port
;        [esp    ] The return address
inb:
    mov dx, [esp + 4]       ; move the address of the I/O port to the dx register
    in  al, dx              ; read a byte from the I/O port and store it in the al register
    ret                     ; return the read byte
```

### io.h
```asm
#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H

/** outb:
 *  Sends the given data to the given I/O port. Defined in io.s
 *
 *  @param port The I/O port to send the data to
 *  @param data The data to send to the I/O port
 */
void outb(unsigned short port, unsigned char data);

/** inb:
 *  Read a byte from an I/O port.
 *
 *  @param  port The address of the I/O port
 *  @return      The read byte
 */
unsigned char inb(unsigned short port);

#endif /* INCLUDE_IO_H */
```

With these functions now defined, we can implement our cursor movement function:
```c
/** fb_move_cursor:
 *  Moves the cursor of the framebuffer to the given position
 *
 *  @param pos The new position of the cursor
 */
void fb_move_cursor(unsigned short pos) {
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT,    ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT,    pos & 0x00FF);
}
```

In addition, add these defines to our `terminal.h`:
```c
/* The I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15
```
A good excrecise is to implement a `write` function that will not only write a string, but also move the cursor:
```c
int write(char *buf, unsigned int len);
```

#Step 3: The Serial Driver

The serial driver is very similar to the VGA driver, and thus will not be explained in detail, however, it is suggested to read the code to see how it works.