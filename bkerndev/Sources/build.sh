#!/bin/bash

CC=i686-elf-gcc

echo "Now assembling, compiling, and linking your kernel:"
#nasm -f aout -o start.o start.asm
nasm -felf32 -o start.o start.asm

#rem Remember this spot here: We will add '$(CC)' commands here to compile C sources

$CC -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./include -c -o main.o main.c

$CC -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./include -c -o scrn.o scrn.c

$CC -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./include -c -o gdt.o gdt.c

$CC -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./include -c -o idt.o idt.c

$CC -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./include -c -o isrs.o isrs.c

$CC -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./include -c -o irq.o irq.c

$CC -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./include -c -o timer.o timer.c

$CC -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./include -c -o kb.o kb.c

#rem This links all your files. Remember that as you add *.o files, you need to
#rem add them after start.o. If you don't add them at all, they won't be in your kernel!
#ld -T link.ld -o kernel.bin start.o main.o scrn.o gdt.o idt.o isrs.o irq.o timer.o kb.o
$CC -T link.ld -o kernel.bin -ffreestanding -O2 -nostdlib *.o -lgcc

echo "Cleaning up object files..."
rm *.o
echo "Done!"
#pause
