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
	ld86 -o out/kernel -d out/kernel.o out/kernel_asm.o out/std_lib.o out/lib_interrupt.o out/utils.o
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
	bcc -ansi -c -o out/string_lib.o src/c/lib/string_lib.c
	bcc -ansi -c -o out/textio.o src/c/lib/textio.c
	bcc -ansi -c -o out/utils.o src/c/lib/utils.c
	bcc -ansi -c -o out/program.o src/c/lib/program.c


shell:
	bcc -ansi -c -o out/shell.o src/c/shell/shell.c
	nasm -f as86 src/asm/interrupt.asm -o out/lib_interrupt.o
	ld86 -o out/shell -d out/shell.o out/lib_interrupt.o out/textio.o out/fileio.o out/program.o out/string_lib.o out/utils.o

cd:
	bcc -ansi -c -o out/cd.o src/c/shell/util/cd.c
	nasm -f as86 src/asm/interrupt.asm -o out/lib_interrupt.o
	ld86 -o out/cd -d out/cd.o out/lib_interrupt.o out/textio.o out/fileio.o out/program.o out/string_lib.o out/utils.o

cp:
	bcc -ansi -c -o out/cp.o src/c/shell/util/cp.c
	nasm -f as86 src/asm/interrupt.asm -o out/lib_interrupt.o
	ld86 -o out/cp -d out/cp.o out/lib_interrupt.o out/textio.o out/fileio.o out/program.o out/string_lib.o out/utils.o

ls:
	bcc -ansi -c -o out/ls.o src/c/shell/util/ls.c
	nasm -f as86 src/asm/interrupt.asm -o out/lib_interrupt.o
	ld86 -o out/ls -d out/ls.o out/lib_interrupt.o out/textio.o out/fileio.o out/program.o out/string_lib.o out/utils.o out/cd.o

mkdir:
	bcc -ansi -c -o out/mkdir.o src/c/shell/util/mkdir.c
	nasm -f as86 src/asm/interrupt.asm -o out/lib_interrupt.o
	ld86 -o out/mkdir -d out/mkdir.o out/lib_interrupt.o out/textio.o out/fileio.o out/program.o out/string_lib.o out/utils.o

mv:
	bcc -ansi -c -o out/mv.o src/c/shell/util/mv.c
	nasm -f as86 src/asm/interrupt.asm -o out/lib_interrupt.o
	ld86 -o out/mv -d out/mv.o out/lib_interrupt.o out/textio.o out/fileio.o out/program.o out/string_lib.o out/utils.o

cat:
	bcc -ansi -c -o out/cat.o src/c/shell/util/cat.c
	nasm -f as86 src/asm/interrupt.asm -o out/lib_interrupt.o
	ld86 -o out/cat -d out/cat.o out/lib_interrupt.o out/textio.o out/fileio.o out/program.o out/string_lib.o out/utils.o

compile_lib: cd cat cp ls mkdir mv

ngetes:
	gcc src/c/shell/shell.c src/c/lib/textio.c -o out/shelltest.o
	nasm -f as86 src/asm/interrupt.asm -o out/lib_interrupt.o
	ld86 -o out/shelltest -d out/shelltest.o out/lib_interrupt.o out/textio.o

test:
	gcc tc_gen.c tc_lib -o tc_gen

testshell: lib compile_lib shell dbk run

run:
	bochs -f src/config/if2230.config

build-run: all run
