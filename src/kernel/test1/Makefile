kernel: kload.o kmain.o Makefile
	ld -o $@ kload.o kmain.o -melf_i386 --oformat elf32-i386 -T link.ld

kmain.o: kmain.c
	gcc -ffreestanding -c $< -o $@ -m32 -fno-pie

kload.o: kload.asm
	nasm -f elf32 -o $@ $<

run: kernel
	qemu-system-i386 --kernel kernel
