## Kosix
### x86 kernel written in C & Assembly

Goals:
- [x] switching into and exiting 32-bit protected mode
   - [x] `int x80` interrupt
- [x] memory segments
- [x] basic paging
- [x] keyboard/screen IO
- [x] PIC timer
- [ ] Memory Manager
- [ ] User Processes
   - [ ] Scheduler
- [ ] minimal POSIX compatibility
- [ ] self-compilation



### Dependencies

- [x] nasm
- [x] i686-elf-gcc cross-compiler from [OSDev cross-compiler wiki page](http://wiki.osdev.org/GCC_Cross-Compiler#Prebuilt_Toolchains) (currently tested gcc used for development: [gcc4.9.1](http://newos.org/toolchains/i686-elf-4.9.1-Linux-x86_64.tar.xz) )
- [ ] xorriso for iso generation: `sudo apt install xorriso1`
   - xorriso doesn't work on my system so I use `qemu-system-i386` to boot `.bin` kernel directly instead of booting `.iso` file from VirtualBox

### Credits

Writing an x86 kernel from scratch is no simple task. If you want to write one yourself, here's some useful resources:

- [bkerndev Bran's tutorial](http://www.osdever.net/bkerndev/Docs/title.htm): I used this tutorial and code for kOSix's base (bootloader, terminal, keyboard input, PIC)
- [OSDev wikie serial port code](http://wiki.osdev.org/Serial_Ports#Sending_data): used for kOSix's serial port code.
- [OSDev Protected Mode/Ring 3](http://wiki.osdev.org/Getting_to_Ring_3): kOSix's code is adopted from this tutorial.
- [Intel processor manuals](https://software.intel.com/en-us/articles/intel-sdm): very long but have essential info
