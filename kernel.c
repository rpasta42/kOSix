
#include "kernel.h"


void kernel_main(void) {
	gdt_install();
	idt_install();

	/* Initialize terminal interface */
	terminal_initialize();

	/* Newline support is left as an exercise. */
	terminal_writestring("Hello, kernel World!\n");
}
