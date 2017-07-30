#ifndef KERNEL_H_INCLUDE
#define KERNEL_H_INCLUDE

//#include <stdbool.h>
//#include <stddef.h>
//#include <stdint.h>
typedef unsigned int size_t;
typedef char uint8_t;
typedef short uint16_t;
typedef unsigned int uint32_t;


/* Check if the compiler thinks we are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* RiddemOS only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

#define PACKED __attribute__((packed))
#define ASM(x) __asm__ __volatile__(x)
#define true 1
#define false 0

/*************************PROTECT*************************/
void _test_user_function();
void set_kernel_stack();
struct gdt_entry;
void setup_protect_segments(struct gdt_entry* gdt);
struct gdt_entry_bits;
void write_tss(struct gdt_entry_bits *g);

void syscall_handler(uint32_t a, uint32_t b, uint32_t c, uint32_t d);

/*********************END PROTECT*************************/


/*************************SERIAL*************************/
void init_serial();
int serial_received();
char read_serial();

int is_transmit_empty();
void write_serial(char a);

/*********************END SERIAL*************************/


/*************************VGA*************************/

/* Hardware text mode color constants. */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}


/*********************END VGA*************************/


/*************************GDT*************************/

#define NUM_GDT 6 //5

void gdt_install();

//http://www.osdever.net/bkerndev/Docs/gdt.htm


/* Defines a GDT entry. We say packed, because it prevents the
*  compiler from doing things that it thinks is best: Prevent
*  compiler "optimization" by packing */
struct gdt_entry {
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access;
    unsigned char granularity;
    unsigned char base_high;
} PACKED;

/* Special pointer which includes the limit: The max bytes
*  taken up by the GDT, minus 1. Again, this NEEDS to be packed */
struct gdt_ptr {
    unsigned short limit;
    unsigned int base;
} PACKED;


extern void gdt_flush();

/*********************END GDT*************************/


/*************************IDT*************************/

void idt_install();

/* Defines an IDT entry */
struct idt_entry {
    unsigned short base_lo;
    unsigned short sel;        /* Our kernel segment goes here! */
    unsigned char always0;     /* This will ALWAYS be set to 0! */
    unsigned char flags;       /* Set using the above table! */
    unsigned short base_hi;
} PACKED;

struct idt_ptr {
    unsigned short limit;
    unsigned int base;
} PACKED;



/*********************END IDT*************************/


/*************************UTILS*************************/

size_t strlen(const char* str);
void* memset(void *dest, char val, size_t count);
void* memcpy(void* dest, const void* src, size_t count);
void print_int(int i);

/*********************END UTILS*************************/


/*************************TERM*************************/

void terminal_initialize(void);
void terminal_writestring(const char* data);

void init_video(void);
void puts(unsigned char *text);

/*********************END TERM*************************/


/*************************ISRS*************************/

void isrs_install();

/* This defines what the stack looks like after an ISR was running */
struct regs {
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */
};


void fault_handler(struct regs *r);

/*********************END ISRS*************************/


/*************************IRQ*************************/

void irq_handler(struct regs *r);
void irq_install();

/*********************END IRQ*************************/


/*************************TIMER*************************/

void timer_handler(struct regs *r);
void timer_install();
void timer_wait(int ticks);

/*********************END TIMER*************************/


/*************************KB*************************/

void keyboard_install();

/*********************END KB*************************/


/*************************PAGING*************************/

void init_paging();

/*********************END PAGING*************************/




#endif //KERNEL_H_INCLUDE

