# Step 1: The Loader
Our loader code does a very simple task. It moves the value `0xDEADBEEF` into the `eax` register. This code is written in AT&T or GAS Assembly syntax. The reason we cannot jump into C code right away is because C requires a stack, which we will set up later.
The following code should be put into a file called `loader.s`:

```asm
global loader                   ; the entry symbol for ELF

MAGIC_NUMBER equ 0x1BADB002     ; define the magic number constant
FLAGS        equ 0x0            ; multiboot flags
CHECKSUM     equ -MAGIC_NUMBER  ; calculate the checksum
								; (magic number + checksum + flags should equal 0)

section .text:                  ; start of the text (code) section
align 4                         ; the code must be 4 byte aligned
    dd MAGIC_NUMBER             ; write the magic number to the machine code,
    dd FLAGS                    ; the flags,
    dd CHECKSUM                 ; and the checksum

loader:                         ; the loader label (defined as entry point in linker script)
    mov eax, 0xDEADBEEF         ; place the number 0xCAFEBABE in the register eax
    .loop:
    jmp .loop                   ; loop forever
```

After this, we can compile this by running the `nasm` assembler:
```bash
nasm -f elf32 loader.s
```


# Step 2: Linking the Kernel
Once we have our "operating system" ready to go, we need to be able to execute it. Instead of just compiling our loader into a executeable, we must make sure that we place our operating system code in the right location. For this, we will create a linker that will place our code where we want it all while leaving space for other system components, specifically GRUB and memory mapped I/O devices.
The following code should be put into a file called `link.ld`

```asm
ENTRY(loader)                /* the name of the entry label */

SECTIONS {
    . = 0x00100000;          /* the code should be loaded at 1 MB */

    .text ALIGN (0x1000) :   /* align at 4 KB */
    {
        *(.text)             /* all text sections from all files */
    }

    .rodata ALIGN (0x1000) : /* align at 4 KB */
    {
        *(.rodata*)          /* all read-only data sections from all files */
    }

    .data ALIGN (0x1000) :   /* align at 4 KB */
    {
        *(.data)             /* all data sections from all files */
    }

    .bss ALIGN (0x1000) :    /* align at 4 KB */
    {
        *(COMMON)            /* all COMMON sections from all files */
        *(.bss)              /* all bss sections from all files */
    }
}
```

And then we can link our kernel code by running a linker:
```
ld -T link.ld -melf_i386 loader.o -o kernel.elf
```

This will produce a `kernel.elf` which is properly linked to run as a OS.

# Step 3: GRUB
Once we have our kernel ready as an executeable, we must have a bootloader so that our computer can transition from BIOS to Kernel. GRUB (in this case, GRUB Legacy) will be acting as our bootloader. A compiled version of GRUB can be found here: 

https://github.com/SIGOPS/osDevTutorial/raw/master/Tutorial%201:%20Booting%20the%20OS/iso/boot/grub/stage2_eltorito

We will avoid manually compiling grub so that we can smoothly continue with the OS development and not have to worry about other pieces.

## Directory setup

At this stage, take some time to organize your files and directory setup for when we begin to automate the build process. Keeping things organized is very important to OS development as it allows for much cleaner development.
Arrange your working directory as such:
```
├── iso
│   └── boot
│       ├── grub
│       │   ├── menu.lst
│       │   └── stage2_eltorito
│       └── kernel.elf
├── link.ld
├── loader.o
├── loader.s
```

All we have done here is create a folder called `iso`. Within `iso` a folder called `boot` was created and finally within `boot` a folder called `grub` was created. Within the `grub` folder, the compiled version of GRUB (stage2_elorito) was put along with a GRUB configuration file: `menu.lst`. This file simply tells GRUB information about our OS and contains this:

```
default=0
timeout=3

title os
kernel /boot/kernel.elf
```

The `kernel.elf` file was also moved to the `boot` folder, just to keep things organized. 


A ISO image can now be generated! From th working directory (the foler with `link.ld` and `loader.s`) run the following command:

```shell
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
```

This runs the program `genisoimage` which will place GRUB and our kernel onto a runnable image.

After this, we can run our OS in a emulator.

#Running Bochs
Create a file named `bochs.txt`. This file will act as our configuration for the Bochs emulator. Within the file, put this text:
```
megs:            32
display_library: sdl
romimage:        file=/usr/share/bochs/BIOS-bochs-latest
vgaromimage:     file=/usr/share/bochs/VGABIOS-lgpl-latest
ata0-master:     type=cdrom, path=os.iso, status=inserted
boot:            cdrom
log:             bochslog.txt
clock:           sync=realtime, time0=local
cpu:             count=1, ips=1000000
```
Findally, run bochs by running
```
bochs -f bochsrc.txt -q
```

and pressing `c` in the console to continue the execution of the OS.

Congrats! You are on your first steps to building a operating system!
