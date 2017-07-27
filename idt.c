#include "kernel.h"

/* This exists in 'boot.asm', and is used to load our IDT */
extern void idt_load();

void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags) {
   idt[num].base_lo = (base & 0xFFFF); //base >> 16;
   idt[num].base_hi = (base >> 16) & 0xFFFF; //(base << 16) >> 16;

   idt[num].sel = sel;
   idt[num].always0 = 0;
   idt[num].flags = flags;
}

void idt_install() {
   /* Sets the special IDT pointer up, just like in 'gdt.c' */
   idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
   idtp.base = &idt;

   /* Clear out the entire IDT, initializing it to zeros */
   memset(&idt, 0, sizeof(struct idt_entry) * 256);

   /* Add any new ISRs to the IDT here using idt_set_gate */

   /* Points the processor's internal register to the new IDT */
   idt_load();
}

