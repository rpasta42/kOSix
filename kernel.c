
#include "kernel.h"


void kernel_main(void) {
   int i;

   gdt_install();
   idt_install();
   isrs_install();
   irq_install();

   /* Initialize terminal interface */
   //terminal_initialize();
   init_video();

   timer_install();
   keyboard_install();

   __asm__ __volatile__ ("sti"); //sti

   //cls();
   puts("Hello World!\n");

   /* Newline support is left as an exercise. */
   //terminal_writestring("Hello, kernel World!\n");

   //i = 10 / 0;
   //putch(i);

   //terminal_writestring("Hello, kernel World!\n");

   while (1) ;
}
