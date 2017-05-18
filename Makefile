all:
	clear; clear; gcc main.c -o emu -lSDL2
run:	all
	./emu
