/* bkerndev - Bran's Kernel Development Tutorial
*  By:   Brandon F. (friesenb@gmail.com)
*  Desc: Global Descriptor Table management
*
*  Notes: No warranty expressed or implied. Use at own risk. */
#include <system.h>

#define __packed __attribute__((packed))
#define NUM_GDT 4

/* Defines a GDT entry */
struct gdt_entry {
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access;
    unsigned char granularity;
    unsigned char base_high;
} __attribute__((packed));

//http://wiki.osdev.org/Getting_to_Ring_3
//************RING 3
struct gdt_entry_bits {
	unsigned int limit_low:16;
	unsigned int base_low : 24;
     //attribute byte split into bitfields
	unsigned int accessed :1;
	unsigned int read_write :1; //readable for code, writable for data
	unsigned int conforming_expand_down :1; //conforming for code, expand down for data
	unsigned int code :1; //1 for code, 0 for data
	unsigned int always_1 :1; //should be 1 for everything but TSS and LDT
	unsigned int DPL :2; //priviledge level
	unsigned int present :1;
     //and now into granularity
	unsigned int limit_high :4;
	unsigned int available :1;
	unsigned int always_0 :1; //should always be 0
	unsigned int big :1; //32bit opcodes for code, uint32_t stack for data
	unsigned int gran :1; //1 to use 4k page addressing, 0 for byte addressing
	unsigned int base_high : 8;
} __attribute__((packed));

typedef struct gdt_entry_bits gdt_entry_bits;

void gdt_ring_3();

void write_tss(gdt_entry_bits *g);


//************END RING 3

struct gdt_ptr {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

/* Our GDT, with 3 entries, and finally our special GDT pointer */
struct gdt_entry gdt[NUM_GDT];
struct gdt_ptr gp;

/* This is in start.asm. We use this to properly reload
*  the new segment registers */
extern void gdt_flush();


/* Setup a descriptor in the Global Descriptor Table */
void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran) {
    /* Setup the descriptor base address */
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    /* Setup the descriptor limits */
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    /* Finally, set up the granularity and access flags */
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

/* Should be called by main. This will setup the special GDT
*  pointer, set up the first 3 entries in our GDT, and then
*  finally call gdt_flush() in our assembler file in order
*  to tell the processor where the new GDT is and update the
*  new segment registers */
void gdt_install() {
   /* Setup the GDT pointer and limit */
   gp.limit = (sizeof(struct gdt_entry) * NUM_GDT) - 1;
   gp.base = &gdt;

   /* Our NULL descriptor */
   gdt_set_gate(0, 0, 0, 0, 0);

   /* The second entry is our Code Segment. The base address
   *  is 0, the limit is 4GBytes, it uses 4KByte granularity,
   *  uses 32-bit opcodes, and is a Code Segment descriptor.
   *  Please check the table above in the tutorial in order
   *  to see exactly what each value means */
   gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

   /* The third entry is our Data Segment. It's EXACTLY the
   *  same as our code segment, but the descriptor type in
   *  this entry's access byte says it's a Data Segment */
   gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

   gdt_ring_3();

   /* Flush out the old GDT and install the new changes! */
   gdt_flush();
}


//************RING 3


//RING 3
void gdt_ring_3() {
   //....insert your ring 0 segments here or whatever
   gdt_entry_bits *code;
   gdt_entry_bits *data;
   //I assume your ring 0 segments are in gdt[1] and gdt[2] (0 is null segment)
   code=(void*)&gdt[3]; //gdt is a static array of gdt_entry_bits or equivalent
   data=(void*)&gdt[4];
   code->limit_low=0xFFFF;
   code->base_low=0;
   code->accessed=0;
   code->read_write=1; //make it readable for code segments
   code->conforming_expand_down=0; //don't worry about this..
   code->code=1; //this is to signal its a code segment
   code->always_1=1;
   code->DPL=3; //set it to ring 3
   code->present=1;
   code->limit_high=0xF;
   code->available=1;
   code->always_0=0;
   code->big=1; //signal it's 32 bits
   code->gran=1; //use 4k page addressing
   code->base_high=0;
   *data=*code; //copy it all over, cause most of it is the same
   data->code=0; //signal it's not code; so it's data.

   write_tss(&gdt[5]); //we'll implement this function later...

   //...go on to install GDT segments and such
   //after those are installed we'll tell the CPU where our TSS is:
   flush_tss(); //implement this later
}


//RING 3
typedef unsigned int  uint32_t;
typedef unsigned short  uint16_t;

// A struct describing a Task State Segment.
struct tss_entry_struct
{
   uint32_t prev_tss;   // The previous TSS - if we used hardware task switching this would form a linked list.
   uint32_t esp0;       // The stack pointer to load when we change to kernel mode.
   uint32_t ss0;        // The stack segment to load when we change to kernel mode.
   uint32_t esp1;       // everything below here is unusued now..
   uint32_t ss1;
   uint32_t esp2;
   uint32_t ss2;
   uint32_t cr3;
   uint32_t eip;
   uint32_t eflags;
   uint32_t eax;
   uint32_t ecx;
   uint32_t edx;
   uint32_t ebx;
   uint32_t esp;
   uint32_t ebp;
   uint32_t esi;
   uint32_t edi;
   uint32_t es;
   uint32_t cs;
   uint32_t ss;
   uint32_t ds;
   uint32_t fs;
   uint32_t gs;
   uint32_t ldt;
   uint16_t trap;
   uint16_t iomap_base;
} __packed;

typedef struct tss_entry_struct tss_entry_t;


/**Ok, this is going to be hackish, but we will salvage the gdt_entry_bits struct to form our TSS descriptor
So some of these names of the fields will actually be different.. maybe I'll fix this later..**/
tss_entry_t tss_entry;

void write_tss(gdt_entry_bits *g)
{
   // Firstly, let's compute the base and limit of our entry into the GDT.
   uint32_t base = (uint32_t) &tss_entry;
   uint32_t limit = sizeof(tss_entry);

   // Now, add our TSS descriptor's address to the GDT.
   g->limit_low=limit&0xFFFF;
   g->base_low=base&0xFFFFFF; //isolate bottom 24 bits
   g->accessed=1; //This indicates it's a TSS and not a LDT. This is a changed meaning
   g->read_write=0; //This indicates if the TSS is busy or not. 0 for not busy
   g->conforming_expand_down=0; //always 0 for TSS
   g->code=1; //For TSS this is 1 for 32bit usage, or 0 for 16bit.
   g->always_1=0; //indicate it is a TSS
   g->DPL=3; //same meaning
   g->present=1; //same meaning
   g->limit_high=(limit&0xF0000)>>16; //isolate top nibble
   g->available=0;
   g->always_0=0; //same thing
   g->big=0; //should leave zero according to manuals. No effect
   g->gran=0; //so that our computed GDT limit is in bytes, not pages
   g->base_high=(base&0xFF000000)>>24; //isolate top byte.

   // Ensure the TSS is initially zero'd.
   memset(&tss_entry, 0, sizeof(tss_entry));

   tss_entry.ss0  = (uint32_t)&gdt[2]; //REPLACE_KERNEL_DATA_SEGMENT;  // Set the kernel stack segment.
   tss_entry.esp0 = (uint32_t)&gdt[1]; //REPLACE_KERNEL_STACK_ADDRESS; // Set the kernel stack pointer.
   //note that CS is loaded from the IDT entry and should be the regular kernel code segment
}


//this will update the ESP0 stack used when an interrupt occurs
void set_kernel_stack(uint32_t stack) {
   tss_entry.esp0 = stack;
}


//************END RING 3

