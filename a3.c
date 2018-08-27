#define WIDTH 2880
#define HEIGHT 1800
#define BPP 4
#define DEPTH 32

#define SCREENSIZE 30
#define VIEWEDFROM 60
#define COORDSTART 0
#define COORDSTOP 399

#include "SDL.h"
#include "gp_repr.h"
#include "gp_trans.h"
#include "gp_render.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


int main () {

	Line2D* dlines = 0;
	RealLine2D* plines = 0;
	RealLine3D* lines = 0;
	
	int num_lines = 12*32 , i, j; // * x for more cubes
	
	// Lets make a square.
	lines = (RealLine3D*)malloc(sizeof(RealLine3D)*num_lines);
	plines = (RealLine2D*)malloc(sizeof(RealLine2D)*num_lines);
	dlines = (Line2D*)malloc(sizeof(Line2D)*num_lines);
	
	/* This is a single cube. */
	lines[0][0][0] = -1;
	lines[0][0][1] = 1;
	lines[0][0][2] = -1;
	lines[0][1][0] = 1;
	lines[0][1][1] = 1;
	lines[0][1][2] = -1;
	lines[1][0][0] = 1;
	lines[1][0][1] = 1;
	lines[1][0][2] = -1;
	lines[1][1][0] = 1;
	lines[1][1][1] = -1;
	lines[1][1][2] = -1;
	lines[2][0][0] = 1;
	lines[2][0][1] = -1;
	lines[2][0][2] = -1;
	lines[2][1][0] = -1;
	lines[2][1][1] = -1;
	lines[2][1][2] = -1;
	lines[3][0][0] = -1;
	lines[3][0][1] = -1;
	lines[3][0][2] = -1;
	lines[3][1][0] = -1;
	lines[3][1][1] = 1;
	lines[3][1][2] = -1;
	lines[4][0][0] = -1;
	lines[4][0][1] = 1;
	lines[4][0][2] = 1;
	lines[4][1][0] = 1;
	lines[4][1][1] = 1;
	lines[4][1][2] = 1;
	lines[5][0][0] = 1;
	lines[5][0][1] = 1;
	lines[5][0][2] = 1;
	lines[5][1][0] = 1;
	lines[5][1][1] = -1;
	lines[5][1][2] = 1;
	lines[6][0][0] = 1;
	lines[6][0][1] = -1;
	lines[6][0][2] = 1;
	lines[6][1][0] = -1;
	lines[6][1][1] = -1;
	lines[6][1][2] = 1;
	lines[7][0][0] = -1;
	lines[7][0][1] = -1;
	lines[7][0][2] = 1;
	lines[7][1][0] = -1;
	lines[7][1][1] = 1;
	lines[7][1][2] = 1;
	lines[8][0][0] = -1;
	lines[8][0][1] = 1;
	lines[8][0][2] = -1;
	lines[8][1][0] = -1;
	lines[8][1][1] = 1;
	lines[8][1][2] = 1;
	lines[9][0][0] = 1;
	lines[9][0][1] = 1;
	lines[9][0][2] = -1;
	lines[9][1][0] = 1;
	lines[9][1][1] = 1;
	lines[9][1][2] = 1;
	lines[10][0][0] = 1;
	lines[10][0][1] = -1;
	lines[10][0][2] = -1;
	lines[10][1][0] = 1;
	lines[10][1][1] = -1;
	lines[10][1][2] = 1;
	lines[11][0][0] = -1;
	lines[11][0][1] = -1;
	lines[11][0][2] = -1;
	lines[11][1][0] = -1;
	lines[11][1][1] = -1;
	lines[11][1][2] = 1;
	
	/* Make a grid of many more cubes. */
	for(i=1; i<num_lines/12; i++) {
		for(j=0; j<12; j++) {
			lines[i*12 + j][0][0] = lines[j][0][0] - (i % 4 - i);//(i % 4 - i) / 2;
			lines[i*12 + j][0][1] = lines[j][0][1] - (i % 4) * 4;//(i % 4 * 2 - i * 2) / 2;
			lines[i*12 + j][0][2] = lines[j][0][2];
			lines[i*12 + j][1][0] = lines[j][1][0] - (i % 4 - i);
			lines[i*12 + j][1][1] = lines[j][1][1] - (i % 4) * 4; //(i % 4 * 2 - i * 2) / 2;
			lines[i*12 + j][1][2] = lines[j][1][2];
		}
	}
	
	RGBColor white = {255, 255, 255}; // Draw Color
	RGBColor black = {0, 0, 0}; // Selective-Erase Color
	
	ViewPort view = {{1, 1}, {2878, 1798}}; // Because round, needs padding.
	RealPoint3D focus = {14.0, -6.0, 5.5};
	RealPoint3D trp = {0.0, 0.0, 0.0}; // Start at Origin looking down X.
	
	int coord = 400;
	float size = 30, dist = 60;
	
	/* Start SDL Surface which is exited on completion or through a key event. ESCAPE */
	SDL_Surface *screen;
	//SDL_Event event;
	SDL_Event keyevent;
	
	
	int escape = 0, space=0, uarr=0, darr=0, one=0, two=0, three=0, q=0, w=0;
	if (SDL_Init(SDL_INIT_VIDEO) < 0 ) return 1;
	if (!(screen = SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH, SDL_FULLSCREEN|SDL_HWSURFACE))) {
	  SDL_Quit();
	  return 1;
	}

	for (i=0; i<num_lines; i++) {
		PerspectiveProjection3D(&plines[i][0], screen, &focus, &lines[i][0], &trp, size, dist, coord);
		PerspectiveProjection3D(&plines[i][1], screen, &focus, &lines[i][1], &trp, size, dist, coord);
	}
	for (i=0; i<num_lines; i++) {
		dlines[i][0][0] = roundf(plines[i][0][0]);
		dlines[i][0][1] = roundf(plines[i][0][1]);
		dlines[i][1][0] = roundf(plines[i][1][0]);
		dlines[i][1][1] = roundf(plines[i][1][1]);
	}


	while (!escape) {
		if (SDL_MUSTLOCK(screen) && SDL_LockSurface(screen) < 0) continue;
		
		if(space || uarr || darr) {
			if (space) {
				dist += 1;
				size += 1;
			}
			if (darr) {
				if(q) {
					if(one)
						focus[0] += 0.1;
					if(two)
						focus[1] += 0.1;
					if(three)
						focus[2] += 0.1;
				}
				if(w) {
					if(one)
						trp[0] += 0.008;
					if(two)
						trp[1] += 0.008;
					if(three)
						trp[2] += 0.008;
				}
			} else if (uarr) {
				if(q) {
					if(one)
						focus[0] -= 0.1;
					if(two)
						focus[1] -= 0.1;
					if(three)
						focus[2] -= 0.1;
				}
				if(w) {
					if(one)
						trp[0] -= 0.008;
					if(two)
						trp[1] -= 0.008;
					if(three)
						trp[2] -= 0.008;
				}
			}
			
			for (i=0; i<num_lines; i++) ClipLine2DAndRender(screen, &view, dlines + i, &black);
			for (i=0; i<num_lines; i++) {
				PerspectiveProjection3D(&plines[i][0], screen, &focus, &lines[i][0], &trp, size, dist, coord);
				PerspectiveProjection3D(&plines[i][1], screen, &focus, &lines[i][1], &trp, size, dist, coord);
			}
			for (i=0; i<num_lines; i++) {
				dlines[i][0][0] = roundf(plines[i][0][0]);
				dlines[i][0][1] = roundf(plines[i][0][1]);
				dlines[i][1][0] = roundf(plines[i][1][0]);
				dlines[i][1][1] = roundf(plines[i][1][1]);
			}
		}
		for (i=0; i<num_lines; i++) ClipLine2DAndRender(screen, &view, dlines + i, &white);
		
		if (SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
		SDL_Flip(screen);

		while (SDL_PollEvent(&keyevent)) {
			if(keyevent.type == SDL_KEYDOWN) {
				switch (keyevent.key.keysym.sym) {
				case SDLK_ESCAPE:
					escape = 1;
					break;
				case SDLK_SPACE:
					space = 1;
					break;
				case SDLK_1:
					one = 1;
					break;
				case SDLK_2:
					two = 1;
					break;
				case SDLK_3:
					three = 1;
					break;
				case SDLK_q:
					q = 1;
					break;
				case SDLK_w:
					w = 1;
					break;
				case SDLK_UP:
					uarr = 1;
					break;
				case SDLK_DOWN:
					darr = 1;
					break;
				default:
					break;
				}
			}
			if(keyevent.type == SDL_KEYUP) {
				switch (keyevent.key.keysym.sym) {
				case SDLK_ESCAPE:
					escape = 1;
					break;
				case SDLK_SPACE:
					space = 0;
					break;
				case SDLK_1:
					one = 0;
					break;
				case SDLK_2:
					two = 0;
					break;
				case SDLK_3:
					three = 0;
					break;
				case SDLK_q:
					q = 0;
					break;
				case SDLK_w:
					w = 0;
					break;
				case SDLK_UP:
					uarr = 0;
					break;
				case SDLK_DOWN:
					darr = 0;
					break;
				default:
					break;
				}
			}
		}
	}
	SDL_Quit();
	
	//for (i=0; i<num_lines && printf("(%f, %f) (%f, %f)\n", plines[i][0][0], plines[i][0][1], plines[i][1][0], plines[i][1][1]); i++);
	
	return 0;
}
