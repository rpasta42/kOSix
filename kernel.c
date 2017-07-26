
#include "kernel.h"


void kernel_main(void) {
   int i;

   gdt_install();
   idt_install();
   isrs_install();

   /* Initialize terminal interface */
   //terminal_initialize();

   init_video();

   __asm__ __volatile__ ("sti");

   cls();
   //puts("Hello World!\n");

   /* Newline support is left as an exercise. */
   //terminal_writestring("Hello, kernel World!\n");

   int x = 5;
   int y = 0;
   int z = x / y;
   putch(z);

   //terminal_writestring("Hello, kernel World!\n");

   while (1) ;
}
