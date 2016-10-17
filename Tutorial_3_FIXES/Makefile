OBJECTS = io.o terminal.o serial.o loader.o kmain.o
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c
LDFLAGS = -T link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf

all: kernel.elf

SRC=$(wildcard *.c)

kernel.elf: $(OBJECTS)
	$(info ************ LINK FILES ************)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf

os.iso: kernel.elf
	$(info ************ ISO GENERATION ************)
	cp kernel.elf iso/boot/kernel.elf
	genisoimage -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -A os -input-charset utf8 -boot-info-table -o os.iso iso

run: os.iso
	$(info ************  RUNNING ************)
	bochs -f bochs.txt -q

%.o: %.c
	$(info ************  COMPILE C FILES ************)
	$(CC) $(CFLAGS)  $< -o $@

%.o: %.s
	$(info ************  COMPILE S FILES ************)
	$(AS) $(ASFLAGS) $< -o $@

clean:
	$(info ************ CLEAN ************)
	rm -rf *.o iso/boot/kernel.elf os.iso bochslog.txt *.out kernel.elf
