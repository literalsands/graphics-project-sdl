#include <stdio.h>
#include "SDL.h"
#include "gp_perform.h"

#define WIDTH 1280
#define HEIGHT 800
#define BPP 4
#define DEPTH 32

/* void draw_SDL_Surface(SDL_Surface*, Routine**); Drawing is setting the screen. */
void plot_PPoint(SDL_Surface*, int*, RGBColor*); /* Plot Pre-computed Y Coordinate. */
void plot_Point(SDL_Surface*, Point2D*, RGBColor*); /* Plot Point where Y is Y * WIDTH. */

int main(int argc, char* argv[]) {
	/* Ask user for input. */
	int brz_line=0, dda_line=0, iteration_cap=0;
	while((!brz_line && !dda_line) || !iteration_cap) {
		printf("What kind of line would you like drawn, and how many? (<'b' or 'd'> <integer>)\n");
		int nbytes = 12;
		char *line;
		line = (char *) malloc (nbytes + 1);
		getline (&line, &nbytes, stdin);
		brz_line = (line[0] == 'b')? 1 : 0;
		dda_line = (line[0] == 'd')? 1 : 0;
		iteration_cap = atoi(line + 2);
	}
	
	/* Start SDL Surface which is exitted on completion or through a key event. */
	SDL_Surface *screen;
	SDL_Event event;
	
	int keypress = 0, iterations = 0;

	if (SDL_Init(SDL_INIT_VIDEO) < 0 ) return 1;
	if (!(screen = SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH, SDL_FULLSCREEN|SDL_HWSURFACE))) {
	  SDL_Quit();
	  return 1;
	}
	while(!keypress && iterations < iteration_cap) {
		if(SDL_MUSTLOCK(screen) && SDL_LockSurface(screen) < 0) return;
		iterations++;
		
		/* Draw a white line. */
		RGBColor color = {255, 255, 255};
		Point2D start = {rand()%WIDTH, rand()%HEIGHT};
		Point2D end = {rand()%WIDTH, rand()%HEIGHT};
		
		/* Declare a Random Dash Length */
		int dash = rand()%25;
		
		Iterator* iterator;
		if(brz_line) {
			iterator = Iterator_Line_BRZ(&start, &end);
		} else if(dda_line) {
			iterator = Iterator_Line_DDA(&start, &end);
		}
		
		/* The current X and Y Coordinates are the first two ([0] and [1]). */
		int * ppoint = iterator->int_args;
		
		/* Draw points at beginning and ending of line. */
		plot_PPoint(screen, ppoint, &color);
		/* Make that dash. */
		int start_dash = 0;
		while((Iterate(iterator))) {
			start_dash++;
			if(start_dash < dash || dash == 0)
				plot_PPoint(screen, ppoint, &color);
			else if(start_dash == (dash << 1))
				start_dash = 0;
		}
		plot_PPoint(screen, ppoint, &color);
		Delete_Iterator(iterator);

		if(SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
		SDL_Flip(screen);
		
		while(SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					  keypress = 1;
					  break;
				case SDL_KEYDOWN:
					  keypress = 1;
					  break;
			}
		}
	}
	
	SDL_Quit();
	
	return 0;
}

/* Plots a point with a precomputed Y. */
void plot_PPoint(SDL_Surface * screen, int * point, RGBColor * color) {
	// Assumes that y has already been multiplied by WIDTH.
	Uint32 *pixmem32 = (Uint32*) screen->pixels + point[1] + point[0];
	*pixmem32 = (Uint32) SDL_MapRGB(screen->format, color->red, color->green, color->blue);
}

void plot_Point(SDL_Surface * screen, Point2D * point, RGBColor * color) {
	// Assumes that y has already been multiplied by WIDTH.
	Uint32 *pixmem32 = (Uint32*) screen->pixels + ((*point)[1] * WIDTH) + (*point)[0];
	*pixmem32 = (Uint32) SDL_MapRGB(screen->format, color->red, color->green, color->blue);
}

