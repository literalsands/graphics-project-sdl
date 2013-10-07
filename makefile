# Makefile for Austin New's Graphics Package
# For Arabnia's Assignment 1

# This Requires SDL to Compile

compile_line_draw: hw1_line_draw.o gp_primitives.o gp_perform.o
	gcc -Wall `sdl-config --libs` hw1_line_draw.o gp_primitives.o -o line_draw -lm
compile_time_test: hw1_time_test.o gp_primitives.o
	gcc -Wall `sdl-config --libs` hw1_time_test.o gp_primitives.o -o time_test -lm
hw1_line_draw.o: gp_primitives.h gp_perform.h hw1_line_draw.c
	gcc -c `sdl-config --cflags` -Wall hw1_line_draw.c
hw1_time_test.o: gp_primitives.h hw1_time_test.c
	gcc -c `sdl-config --cflags` -Wall hw1_time_test.c
gp_perform.o: gp_primitives.h gp_primitives.c
	gcc -c -Wall gp_primitives.c
gp_primitives.o: gp_primitives.h gp_primitives.c
	gcc -c -Wall gp_primitives.c
clean:
	\rm *.o time_test line_draw
