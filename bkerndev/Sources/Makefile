
#run: source ./setup_cross_comp.sh


default: test

test: kernel.bin
	qemu-system-i386 -kernel kernel.bin

kernel.bin:
	./build.sh

clean:
	rm -f *.iso *.o *.bin isodir/boot/*.bin


