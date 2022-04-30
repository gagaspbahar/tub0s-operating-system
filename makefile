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
	ld86 -o out/kernel -d out/kernel.o out/kernel_asm.o out/std_lib.o out/lib_interrupt.o
	dd if=out/kernel of=out/system.img bs=512 conv=notrunc seek=1

dbk: diskimage bootloader kernel

stdlib:
	bcc -ansi -c -o out/std_lib.o src/c/std_lib.c

gen:
	gcc src/c/gen.c tc_gen/tc_lib -o out/gen

generate: gen
	cd out && ./gen system.img

lib:
	bcc -ansi -c -o out/fileio.o src/c/lib/fileio.c
	bcc -ansi -c -o out/math.o src/c/lib/math.c
	bcc -ansi -c -o out/program.o src/c/lib/program.c
	bcc -ansi -c -o out/string.o src/c/lib/string.c
	bcc -ansi -c -o out/textio.o src/c/lib/textio.c
	bcc -ansi -c -o out/utils.o src/c/lib/utils.c


shell:
	bcc -ansi -c -o out/shell.o src/c/shell/shell.c
	bcc -ansi -c -o out/textio.o src/c/lib/textio.c
	nasm -f as86 src/asm/interrupt.asm -o out/lib_interrupt.o
	ld86 -o out/shell -d out/shell.o out/lib_interrupt.o out/textio.o

ngetes:
	gcc src/c/shell/shell.c src/c/lib/textio.c -o out/shelltest.o
	nasm -f as86 src/asm/interrupt.asm -o out/lib_interrupt.o
	ld86 -o out/shelltest -d out/shelltest.o out/lib_interrupt.o out/textio.o


test:
	gcc tc_gen.c tc_lib -o tc_gen

testshell: shell dbk run

run:
	bochs -f src/config/if2230.config

build-run: all run
