#include "gp_primitives.h"
#include <stdlib.h>
#include <math.h>

/* Bresenham's Line Iteration Algorythm */
Iterator* Iterator_Line_BRZ(Point2D* start, Point2D* end) {
	Iterator * brz_iterator = (Iterator*)malloc(sizeof(Iterator));
	
	int * int_args = (int*)malloc(sizeof(int)*10);
	brz_iterator->int_args = int_args;
	
	/* X0 = Start[x] */
	int_args[0] = (*start)[0];
	/* Y0 = Start[y] */
	int_args[1] = (*start)[1] * WIDTH;
	/* X1 = End[x] */
	int_args[2] = (*end)[0];
	/* Y1 = End[y] */
	int_args[3] = (*end)[1] * WIDTH;
	/* DX = abs(X1 - X0) (Change of X) */
	int_args[4] = abs((*end)[0] - (*start)[0]);
	/* -DY = -abs(Y1 - Y0) (Change of Y) */
	int_args[5] = -abs((*end)[1] - (*start)[1]);
	/* X Increment */
	int_args[6] = ((*start)[0] < (*end)[0])? 1 : -1;
	/* Y Increment */
	int_args[7] = ((*start)[1] < (*end)[1])? WIDTH : -WIDTH;
	/* Error Value */
	int_args[8] = int_args[4] + int_args[5];
	/* ErrorShift (For Generalized Bresenham's Algorythm)
	int_args[9] = (int_args[8] << 1); 
		Only Used in function. Memory for variable is this way pre-allocated.
	*/
	
	/* Make sure this pointer is NULL. Otherwise Memory Errors on Delete. */
	brz_iterator->float_args = 0;
	
	brz_iterator->iteration_function = BRZ_Line_Iteration;
	
	return brz_iterator;
}

int BRZ_Line_Iteration(int* int_args, float* float_args) {
	/* If Both X and Y are Fully Incremented, Final Iteration Reached */
	if(int_args[0] == int_args[2] && int_args[1] == int_args[3])
		return 0;
	
	/* (Error * 2) */	
	int_args[9] = (int_args[8] << 1);
	/* If ErrorShift is greater than -DY */
	if (int_args[9] > int_args[5]) { 
		/* Error += -DY */
		int_args[8] += int_args[5];
		/* Increment X */
		int_args[0] += int_args[6];
	}
	/* If ErrorShift is greater than DX */
	if (int_args[9] < int_args[4]) {
		/* Error += DX */
		int_args[8] += int_args[4];
		/* Increment Y */
		int_args[1] += int_args[7]; 
	}
	return 1;
}

/* Digital Differential Analyzer Algorythm */
Iterator* Iterator_Line_DDA(Point2D* start, Point2D* end) {
	Iterator * dda_iterator = (Iterator*)malloc(sizeof(Iterator));
	
	int * int_args = (int*)malloc(sizeof(int)*8);
	dda_iterator->int_args = int_args;
	
	/* X0 = Start[x] */
	int_args[0] = (*start)[0];
	/* Y0 = Start[y] */
	int_args[1] = (*start)[1] * WIDTH;
	/* X1 = End[x] */
	int_args[2] = (*end)[0];
	/* Y1 = End[y] */
	int_args[3] = (*end)[1] * WIDTH;
	/* DX = X1 - X0 (Change of X) */
	int_args[4] = (*end)[0] - (*start)[0];
	/* DY = Y1 - Y0 (Change of Y) */
	int_args[5] = (*end)[1] - (*start)[1];
	/* Y Has Greater Change */
	int_args[6] = (abs(int_args[4]) < abs(int_args[5]));
	/* Increment Value for X or Y of Greater Change */
	int_args[7] = (int_args[6])?
		/* Y of Greater Change */
		((int_args[5] > 0)? WIDTH: -WIDTH):
		/* X of Greater Change */
		((int_args[4] > 0)? 1: -1);
	
	float * float_args = (float*)malloc(sizeof(float)*2);
	dda_iterator->float_args = float_args;
	
	/* X or Y of Lesser Change Float Representation */
	float_args[0] = (int_args[6])? (float) int_args[0]: (float) (*start)[1];
	/* X or Y of Lesser Change Float Increment */
	float_args[1] = (int_args[6])?
		/* X of Lesser Change */
		(float) abs(int_args[4]) / (float) abs(int_args[5]) * ((int_args[4] > 0)? 1.0: -1.0):
		/* Y of Lesser Change */
		(float) abs(int_args[5]) / (float) abs(int_args[4]) * ((int_args[5] > 0)? 1.0: -1.0);

	dda_iterator->iteration_function = DDA_Line_Iteration;

	return dda_iterator;
}

int DDA_Line_Iteration(int* int_args, float* float_args) {
	/* If Variable of Greater Change has Incremented Fully, Final Iteration Reached */
	if((int_args[0] == int_args[2] && !int_args[6]) || (int_args[1] == int_args[3] && int_args[6]))
		return 0;

	/* Increment Floating Representation of Variable of Lesser Change */
	float_args[0] += float_args[1];
	
	/* If Y of Greater Change */
	if(int_args[6]) {
		/* X += X Increment */
		int_args[1] += int_args[7];
		/* Y = trunc(Y Float Representation) */
		int_args[0] = roundf(float_args[0]);
	/* If X of Greater Change */
	} else {
		/* Y += Y Increment */
		int_args[0] += int_args[7];
		/* X = trunc(X Float Representation) */
		int_args[1] = roundf(float_args[0]) * WIDTH;
	}
	return 1;
}

/* This Circle Iterator isn't quite finished... */
Iterator* Iterator_Circle_BRZ(Point2D* point, int radius) {
	Iterator * brz_iterator = (Iterator*)malloc(sizeof(Iterator));
	
	int * int_args = (int*)malloc(sizeof(int)*radius*5);
	brz_iterator->int_args = int_args;
	
	int_args[2] = (*point)[0] + 0;
	int_args[3] = (*point)[1] * WIDTH;
	int_args[4] = 0;
	int_args[5] = radius;
	int_args[0] = int_args[2] + int_args[0];
	int_args[1] = int_args[3] + (int_args[1] * WIDTH);
	int_args[6] = 3 - (radius << 1);
	
	brz_iterator->iteration_function = BRZ_Circle_Iteration;
	return brz_iterator;
}

int BRZ_Circle_Iteration(int* int_args, float* float_args) {
	if(int_args[4]<int_args[5]) {
		if(int_args[6] < 0) {
			int_args[6] += (int_args[4] << 2) + 6;
		} else {
			int_args[6] += ((int_args[4]-int_args[5]) << 2) + 10;
			int_args[5]--;
		}
		int_args[4]++;
		// Put Point into Queue;
		return 1;
	} else
		return 0;
}

int Iterate(Iterator* iterator) {
	/* Signifies one 'loop' if an iteration function and its variables. */
	return iterator->iteration_function(iterator->int_args, iterator->float_args);
}

void Delete_Iterator(Iterator* del) {
	free(del->int_args);
	free(del->float_args);
	free(del);
}
