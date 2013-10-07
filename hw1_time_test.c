#include "gp_primitives.h"
#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"

int main(int argc, char* argv[]) {
	if(argc < 3) {
		printf("Requires two arguments, iterations, and lines per iteration.\n");
		exit(1);
	}
	int iterations=0, iteration_cap=atoi(argv[1]), line_cap=atoi(argv[2]);
	while(iterations < iteration_cap) {
		Uint32 time_taken_BRZ = 0;
		Uint32 time_taken_DDA = 0;
		iterations++;
		int lines=0;
		while(lines < line_cap) {
			lines++;
			Point2D start = {rand()%WIDTH, rand()%HEIGHT};
			Point2D end = {rand()%WIDTH, rand()%HEIGHT};
			Iterator* iterator;
			Uint32 start_time = 0;
			
			start_time = SDL_GetTicks();
			iterator = Iterator_Line_BRZ(&start, &end);
			while((Iterate(iterator)));
			time_taken_BRZ += SDL_GetTicks() - start_time;
			
			Delete_Iterator(iterator);
			
			start_time = SDL_GetTicks();
			iterator = Iterator_Line_DDA(&start, &end);
			while((Iterate(iterator)));
			time_taken_DDA += SDL_GetTicks() - start_time;
			
			Delete_Iterator(iterator);
		}
		printf("BRZ\t%i\tDDA\t%i\n", time_taken_BRZ, time_taken_DDA);
	}
	return 0;
}
