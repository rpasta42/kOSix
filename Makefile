
#run: source ./setup_cross_comp.sh


CC=i686-elf-gcc
#CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra
CFLAGS=-fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin

default: test

test: clean kosix.bin
	qemu-system-i386 -kernel kosix.bin -serial file:kosix.log

#test: iso
#	qemu-system-i386 -cdrom kosix.iso

iso: kosix.bin
	mv kosix.bin isodir/boot/; grub-mkrescue -o kosix.iso isodir


%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)


kosix.bin: gdt.o utils.o term.o idt.o kernel.o boot.o isrs.o kb.o irq.o timer.o paging.o protect.o serial.o
	$(CC) -T linker.ld -o kosix.bin -ffreestanding -O2 -nostdlib *.o -lgcc


#kernel.o: #setup-cross-comp
#	$(CC) -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

boot.o:
	nasm -felf32 boot.asm -o boot.o

clean:
	rm -f *.iso *.o *.bin isodir/boot/*.bin


