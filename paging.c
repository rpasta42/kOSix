#include "kernel.h"

/* page directory */
struct page_dir_entry {

} PACKED;

/* page table */
struct page_table_entry {

} PACKED;



extern void load_page_directory(unsigned int*);
extern void enable_paging();

uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t page_table1[1024] __attribute__((aligned(4096))); /* kernel */
uint32_t page_table2[1024] __attribute__((aligned(4096))); /* user */


void init_paging() {
   int i;
   for (i = 0; i < 1024; i++) {
      //Supervisor: only kernel-mode
      //Write-Enabled: It can be both read from and written to
      //not Present:
      page_directory[i] = 0x00000002;
   }

   for (i = 0; i < 1024; i++) {
      page_table1[i] = (i * 0x1000) | 3; //supervisor level, read/write, present
      page_table2[i] = (i * 0x1000) | 7; //user level, read/write, present
   }

   page_directory[0] = ((unsigned int)page_table1) | 3;
   page_directory[1] = ((unsigned int)page_table2) | 7;

   load_page_directory(page_directory);
   enable_paging();
}
