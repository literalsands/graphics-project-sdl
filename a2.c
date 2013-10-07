#define WIDTH 1280
#define HEIGHT 800
#define BPP 4
#define DEPTH 32

#include "SDL.h"
#include "gp_repr.h"
#include "gp_trans.h"
#include "gp_render.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


int main () {

	FILE* read_from;
	FILE* write_to;
	Line2D* dlines = 0;
	RealLine2D* lines = 0;
	char file_name[256], input[256];
	int num_lines = 0, i;
	
	RGBColor white = {255, 255, 255}; // Draw Color
	RGBColor black = {0, 0, 0}; // Selective-Erase Color
	
	ViewPort view = {{1, 1}, {1278, 798}};
	ViewPort* vp = &view;
	Point2D pivot = {view[0][0] + ((view[1][0] - view[0][0]) >> 1), view[0][1] + ((view[1][1] - view[0][1]) >> 1)};
	TransformMatrix2D start_matrix = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
	TransformMatrix2D* matrix = &start_matrix;
	
	
	/* Start SDL Surface which is exited on completion or through a key event. */
	SDL_Surface *screen;
	//SDL_Event event;
	SDL_Event keyevent;
	
	while(1) {
		/* Ask user for initial input. */
		
		printf("\n[inpt]\tInput lines from a file.\n");
		if(num_lines > 0) {
			printf("[oupt]\tOutput lines to a file.\n");
			printf("[trfm]\tCreate a new transformation.\n");
			printf("[vwpt]\tChange the viewport.\n");
			printf("[view]\tView the current image.\n");
		}
		printf("[quit]\tExit.\n\n");
		printf("\n[????]\tWhat would you like to do?\n\n?:");
	
		if(!scanf("%s", input)) {
			printf("Something's wrong!\n");
			exit(1);
		}
	
		printf("\n");
		if(!strncmp(input, "inpt", 256)) {
			printf("Please specify a file to input lines from.\n");
			if(scanf("%s", file_name) && (read_from = fopen(file_name, "r"))) {
				printf("Got file open.\n");
				if(fscanf(read_from, "%i\n", &num_lines)) {
					dlines = (Line2D*)malloc(sizeof(Line2D)*num_lines);
					for (i=0; i<num_lines && fscanf(read_from, "%i %i %i %i\n", &dlines[i][0][0], &dlines[i][0][1], &dlines[i][1][0], &dlines[i][1][1]); i++);
					lines = (RealLine2D*)malloc(sizeof(RealLine2D)*num_lines);
					for (i=0; i<num_lines; i++) {
						lines[i][0][0] = 0.0 + (float) dlines[i][0][0];
						lines[i][0][1] = 0.0 + (float) dlines[i][0][1];
						lines[i][1][0] = 0.0 + (float) dlines[i][1][0];
						lines[i][1][1] = 0.0 + (float) dlines[i][1][1];
					}
					for (i=0; i<num_lines && printf("Read Line (%i, %i) (%i, %i)\n", dlines[i][0][0], dlines[i][0][1], dlines[i][1][0], dlines[i][1][1]); i++);
					fclose(read_from);
				} else
					printf("File doesn't contain an n-lines header.\n");
			} else
				printf("File was not loaded successfully.\n");
			
		} else if (!strncmp(input, "oupt", 256) && num_lines > 0 && dlines) {
			printf("Please specify a file to output lines to.\n");
			if(scanf("%s", file_name) && (write_to = fopen(file_name, "w"))) {
				printf("Got file open.\n");
				fprintf(write_to, "%i\n", num_lines);
				for (i=0; i<num_lines && fprintf(write_to, "%i %i %i %i\n", dlines[i][0][0], dlines[i][0][1], dlines[i][1][0], dlines[i][1][1]); i++);
				printf("Wrote %i lines to %s!\n", num_lines, file_name);
				fclose(write_to);
			} else 	
				printf("File was not loaded successfully.\n");
	
		} else if (!strncmp(input, "trfm", 256) && num_lines > 0) {
			float a1=0.0, a2=0.0;
			char trfm_input[256];
			int j;
			for(i=0; i<3; i++)
				for(j=0; j<3; j++)
					if(i==j)
						start_matrix[i][j] = 1.0;
					else
						start_matrix[i][j] = 0.0;
			TranslateTransformMatrix2D(matrix, -pivot[0], -pivot[1]);
			printf("[????] What transformations would you like?\n\n");
			printf("[tran] Translate\n");
			printf("[rota] Rotate\n");
			printf("[scal] Scale\n");
			printf("[stop] Finish Transformation\n");
			while(1) {
				scanf("%s", trfm_input);
				if(!strncmp(trfm_input, "stop", 256))
					break;
				else if(!strncmp(trfm_input, "tran", 256)) {
					printf("<sx> <sy>\n");
					scanf("%f %f", &a1, &a2);
					TranslateTransformMatrix2D(matrix, roundf(a1), roundf(a2));
				} else if(!strncmp(trfm_input, "rota", 256)) {
					printf("<theta>\n");
					scanf("%f", &a1);
					RotateTransformMatrix2D(matrix, a1);
				} else if(!strncmp(trfm_input, "scal", 256)) {
					printf("<sx> <sy>\n");
					scanf("%f %f", &a1, &a2);
					ScaleTransformMatrix2D(matrix, a1, a2);
				}
				printf("More?\n");
			}
			TranslateTransformMatrix2D(matrix, pivot[0], pivot[1]);
			for (i=0; i<num_lines; i++) {
				Point2DTransformation(&lines[i][0], matrix);
				Point2DTransformation(&lines[i][1], matrix);
			}
			for (i=0; i<num_lines; i++) {
				dlines[i][0][0] = roundf(lines[i][0][0]);
				dlines[i][0][1] = roundf(lines[i][0][1]);
				dlines[i][1][0] = roundf(lines[i][1][0]);
				dlines[i][1][1] = roundf(lines[i][1][1]);
			}
		
		} else if (!strncmp(input, "vwpt", 256) && num_lines > 0) {
			int t=0, l=0, b=1280, r=800;
			printf("Please write the viewport's top, left, bottom, and right coodinates.\n");
			printf("<top> <left> <bottom> <right>\n");
			printf("(The default is: 0 0 1280 800)\n");
			while (scanf("%i %i %i %i", &l, &t, &r, &b) && !(t >= 0 && l >= 0 && b <= HEIGHT && r <= WIDTH && t < b && l < r))
				printf("You've made an error!\n");
			view[0][0] = l;
			view[0][1] = t;
			view[1][0] = r;
			view[1][1] = b;
		
		} else if (!strncmp(input, "view", 256) && num_lines > 0 && lines && dlines) {
			int escape = 0, space=0;
			if (SDL_Init(SDL_INIT_VIDEO) < 0 ) return 1;
			if (!(screen = SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH, SDL_FULLSCREEN|SDL_HWSURFACE))) {
			  SDL_Quit();
			  return 1;
			}

			while (!escape) {
				if (SDL_MUSTLOCK(screen) && SDL_LockSurface(screen) < 0) continue;
				
				if(space) {
					for (i=0; i<num_lines; i++) ClipLine2DAndRender(screen, vp, dlines + i, &black);
					for (i=0; i<num_lines; i++) {
						Point2DTransformation(&lines[i][0], matrix);
						Point2DTransformation(&lines[i][1], matrix);
					}
					for (i=0; i<num_lines; i++) {
						dlines[i][0][0] = roundf(lines[i][0][0]);
						dlines[i][0][1] = roundf(lines[i][0][1]);
						dlines[i][1][0] = roundf(lines[i][1][0]);
						dlines[i][1][1] = roundf(lines[i][1][1]);
					}
				}
				for (i=0; i<num_lines; i++) ClipLine2DAndRender(screen, vp, dlines + i, &white);
				
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
						default:
							break;
						}
					}
				}
			}
			SDL_Quit();
		
		} else if (!strncmp(input, "quit", 256))
			return 0;
	}
	return 0;
}
