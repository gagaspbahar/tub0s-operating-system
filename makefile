# Makefile
all: diskimage bootloader stdlib shell kernel

# Recipes
diskimage:
	dd if=/dev/zero of=out/system.img bs=512 count=2880

bootloader:
	nasm src/asm/bootloader.asm -o out/bootloader
	dd if=out/bootloader of=out/system.img bs=512 count=1 conv=notrunc

kernel:
	bcc -ansi -c -o out/kernel.o src/c/kernel.c
	nasm -f as86 src/asm/kernel.asm -o out/kernel_asm.o
	nasm -f as86 src/asm/interrupt.asm -o out/lib_interrupt.o
	ld86 -o out/kernel -d out/kernel.o out/kernel_asm.o out/std_lib.o out/shell.o out/lib_interrupt.o
	dd if=out/kernel of=out/system.img bs=512 conv=notrunc seek=1

stdlib:
	bcc -ansi -c -o out/std_lib.o src/c/std_lib.c

shell:
	bcc -ansi -c -o out/shell.o src/c/shell.c

test:
	gcc tc_gen.c tc_lib -o tc_gen

run:
	bochs -f src/config/if2230.config

build-run: all run
