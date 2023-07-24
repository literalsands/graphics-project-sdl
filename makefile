# Makefile for Austin New's Graphics Package
# For Arabnia's Assignment 2

# This Requires SDL with Development Headers to Compile

compile: a3.o gp_trans.o gp_render.o
	gcc -Wall a3.o gp_trans.o gp_render.o -o a3 -lm `sdl-config --libs`
a3.o: gp_repr.h a3.c gp_trans.o gp_render.o
	gcc -c -Wall a3.c `sdl-config --cflags` 
gp_trans.o: gp_repr.h gp_trans.h gp_trans.c
	gcc -c -Wall gp_trans.c
gp_render.o: gp_repr.h gp_render.h gp_render.c
	gcc -c -Wall gp_render.c `sdl-config --cflags` 
clean:
	\rm *.o a3
