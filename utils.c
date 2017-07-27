#include "kernel.h"

size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len] && str[len] != '\0')
		len++;
	return len;
}

void* memcpy(void* dest, const void* src, size_t count) {
    /* Add code here to copy 'count' bytes of data from 'src' to
    *  'dest', finally return 'dest' */
    const char *sp = (const char*)src;
    char *dp = (char*)dest;
    for(; count != 0; count--) *dp++ = *sp++;
    return dest;
}

void* memset(void *dest, char val, size_t count) {
      /*int i;
      for (i = 0; i < count; i++) {
            dest[i] = val;
      }
      return dest;*/
      char *temp = (char*)dest;
      for(; count != 0; count--) *temp++ = val;
      return dest;

    /* Add code here to set 'count' bytes in 'dest' to 'val'.
    *  Again, return 'dest' */
}

unsigned short *memsetw(unsigned short *dest, unsigned short val, int count) {
    /* Same as above, but this time, we're working with a 16-bit
    *  'val' and dest pointer. Your code can be an exact copy of
    *  the above, provided that your local variables if any, are
    *  unsigned short */
    unsigned short *temp = (unsigned short *)dest;
    for (; count != 0; count--) *temp++ = val;
    return dest;
}




/* We will use this later on for reading from the I/O ports to get data
*  from devices such as the keyboard. We are using what is called
*  'inline assembly' in these routines to actually do the work */
unsigned char inportb (unsigned short _port) {
   unsigned char rv;
   __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
   return rv;
}

/* We will use this to write to I/O ports to send bytes to devices. This
*  will be used in the next tutorial for changing the textmode cursor
*  position. Again, we use some inline assembly for the stuff that simply
*  cannot be done in C */
void outportb (unsigned short _port, unsigned char _data) {
   __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

char digit_to_str(int i) {
   return (char)i + '0';
}

void print_int(int i) {
   if (i < 0)
      putch('-');

   //if
}


