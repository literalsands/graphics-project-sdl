# Makefile for Austin New's Graphics Package
# For Arabnia's Assignment 2

# This Requires SDL with Development Headers to Compile

compile: a3.o gp_trans.o gp_render.o
	gcc -Wall `sdl-config --libs` a3.o gp_trans.o gp_render.o -o a3 -lm
a3.o: gp_repr.h a3.c gp_trans.o gp_render.o
	gcc -c -Wall `sdl-config --cflags` a3.c
gp_trans.o: gp_repr.h gp_trans.h gp_trans.c
	gcc -c -Wall gp_trans.c
gp_render.o: gp_repr.h gp_render.h gp_render.c
	gcc -c -Wall `sdl-config --cflags` gp_render.c
clean:
	\rm *.o a3
