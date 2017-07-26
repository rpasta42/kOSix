
#run: source ./setup_cross_comp.sh

default: test

test: kosix.bin
	qemu-system-i386 -kernel kosix.bin

#test: iso
#	qemu-system-i386 -cdrom kosix.iso

#iso: kosix.bin
#	mv kosix.bin isodir/boot/; grub-mkrescue -o kosix.iso isodir

kosix.bin: kernel.o boot.o
	i686-elf-gcc -T linker.ld -o kosix.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc

kernel.o: #setup-cross-comp
	i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

boot.o:
	nasm -felf32 boot.asm -o boot.o

clean:
	rm -f *.iso *.o *.bin iso/boot/*.bin


