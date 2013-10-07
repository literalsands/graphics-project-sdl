# Makefile for Austin New's Graphics Package
# For Arabnia's Assignment 2

# This Requires SDL with Development Headers to Compile

compile: a2.o gp_trans.o gp_render.o
	gcc -g -Wall `sdl-config --libs` a2.o gp_trans.o gp_render.o -o a2 -lm
a2.o: gp_repr.h a2.c gp_trans.o gp_render.o
	gcc -c -Wall `sdl-config --cflags` a2.c
gp_trans.o: gp_repr.h gp_trans.h gp_trans.c
	gcc -c -Wall gp_trans.c
gp_render.o: gp_repr.h gp_render.h gp_render.c
	gcc -c -Wall `sdl-config --cflags` gp_render.c
clean:
	\rm *.o a2
