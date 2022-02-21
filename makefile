# Makefile
all: diskimage bootloader stdlib kernel

# Recipes
diskimage:
	# TODO : Tambahkan untuk pembuatan image

bootloader:
	# TODO : Tambahkan untuk pembuatan bootloader

kernel:
	# TODO : Tambahkan untuk pembuatan kernel

stdlib:
	# Opsional

run:
	bochs -f src/config/if2230.config

build-run: all run
