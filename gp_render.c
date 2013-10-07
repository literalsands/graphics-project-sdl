#include "SDL.h"
#include "gp_repr.h"
#include "gp_render.h"
#include <math.h>

/* Plots a point with a precomputed Y. */
void plot_PPoint(SDL_Surface * screen, int * point, RGBColor* color) {
	// Assumes that y has already been multiplied by WIDTH.
	Uint32 *pixmem32 = (Uint32*) screen->pixels + point[1] + point[0];
	*pixmem32 = (Uint32) SDL_MapRGB(screen->format, (*color)[0], (*color)[1], (*color)[2]);
}

void plot_Point(SDL_Surface * screen, Point2D* point, RGBColor* color) {
	int computed_point = (*point)[0] + (*point)[1] * screen->w;
	plot_PPoint(screen, &computed_point, color);
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

/* TODO Optimize Bresenham Iterator with Multiple Functions */
/* Bresenham's Line Iteration Algorythm */
Iterator* Iterator_Line2D(SDL_Surface * screen, Point2D* start, Point2D* end) {
	Iterator * brz_iterator = (Iterator*)malloc(sizeof(Iterator));
	
	int * int_args = (int*)malloc(sizeof(int)*10);
	brz_iterator->int_args = int_args;
	
	/* X0 = Start[x] */
	int_args[0] = (*start)[0];
	/* Y0 = Start[y] */
	int_args[1] = (*start)[1] * screen->w;
	/* X1 = End[x] */
	int_args[2] = (*end)[0];
	/* Y1 = End[y] */
	int_args[3] = (*end)[1] * screen->w;
	/* DX = abs(X1 - X0) (Change of X) */
	int_args[4] = abs((*end)[0] - (*start)[0]);
	/* -DY = -abs(Y1 - Y0) (Change of Y) */
	int_args[5] = -abs((*end)[1] - (*start)[1]);
	/* X Increment */
	int_args[6] = ((*start)[0] < (*end)[0])? 1 : -1;
	/* Y Increment */
	int_args[7] = ((*start)[1] < (*end)[1])? screen->w : -(screen->w);
	/* Error Value */
	int_args[8] = int_args[4] + int_args[5];
	/* ErrorShift (For Generalized Bresenham's Algorythm)
	int_args[9] = (int_args[8] << 1); 
		Only Used in function. Memory for variable is this way pre-allocated.
	*/
	
	/* Make sure this pointer is NULL. Otherwise Memory Errors on Delete. */
	brz_iterator->float_args = 0;
	
	brz_iterator->iteration_function = Line2D_Iteration;
	
	return brz_iterator;
}

int Line2D_Iteration(int* int_args, float* float_args) {
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

void setViewPort(SDL_Surface* screen, ViewPort* vp, int top, int left, int bottom, int right) {
	if(top >= 0 && left >= 0 && bottom <= screen->h && right <= screen->w) {
		(*vp)[0][0] = top;
		(*vp)[0][1] = left;
		(*vp)[1][0] = bottom;
		(*vp)[1][1] = right;
	}
	/* TODO Compute Resizing Ratio for Window to Viewport Transformation
		Shift-Up, Shift-Left, Width-Ratio, Height-Ratio
	 */
}

void ClipLine2DAndRender(SDL_Surface* screen, ViewPort* vp, Line2D* line, RGBColor* color) {
	// Liang-Barsky Line Clipping Algorythm
	// According to Wikipedia, this is much faster than the Cohen-Sutherland Algorythm
	float dx = (*line)[1][0] - (*line)[0][0];
	float dy = (*line)[1][1] - (*line)[0][1];
	float p, q, r, t0=0.0, t1=1.0;
	int edge;
	
	for (edge=0; edge<4; edge++) {
		switch (edge) {
			case 0:
				p = -dx;
				q = -(*vp)[0][0] + (*line)[0][0];
				break;
			case 1:
				p = dx;
				q = (*vp)[1][0] - (*line)[0][0];
				break;
			case 2:
				p = -dy;
				q = -(*vp)[0][1] + (*line)[0][1];
				break;
			case 3:
				p = dy;
				q = (*vp)[1][1] - (*line)[0][1];
				break;
		}
	
		r = q / p;
		
		if (p<0) {
			if (r>t1)
				return;
			else if (r>t0)
				t0 = r;
		} else if (p>0) {
			if (r<t0)
				return;
			else if (r<t1)
				t1 = r;
		} else if (q<0)
			return;
	}
	
	
	static Line2D d_line;
	static Iterator* iterator;
	static int* ppoint;
	
	d_line[0][0] = (*line)[0][0] + truncf(t0 * dx);
	d_line[0][1] = (*line)[0][1] + truncf(t0 * dy);
	d_line[1][0] = (*line)[0][0] + truncf(t1 * dx);
	d_line[1][1] = (*line)[0][1] + truncf(t1 * dy);
	
	/* TODO Perform Window to Viewport Transformation */
	
	iterator = Iterator_Line2D(screen, (Point2D*) &d_line[0][0], (Point2D*) &d_line[1][0]);
	
	/* The current X and Y Coordinates are the first two ([0] and [1]). */
	ppoint = iterator->int_args;
	
	/* Draw points at beginning and ending of line. */
	plot_PPoint(screen, ppoint, color);
	while((Iterate(iterator)))
		plot_PPoint(screen, ppoint, color);
	plot_PPoint(screen, ppoint, color);
	
	Delete_Iterator(iterator);
	
	return;
}

void PerspectiveProjection3D(RealPoint2D* ppoint, SDL_Surface* screen, RealPoint3D* focus, RealPoint3D* point, float reals, float reald) {
	/* Note:
			real size and real distance are acknowledged to be in the same measurement.
			The camera is has no 'tilt', 'roll', or 'pitch'.
	*/
	RealPoint3D trp = {0.0, 0.0, 0.0};
	float wcs = reals/reald;
	float cx=cos(trp[0]), cy=cos(trp[1]), cz=cos(trp[2]),
		sx=sin(trp[0]), sy=sin(trp[1]), sz=sin(trp[2]);
	float ddx = (*point)[0] - (*focus)[0],
		ddy = (*point)[1] - (*focus)[1],
		ddz = (*point)[2] - (*focus)[2];
	float sub_yz1 = cy * ddz + sy * (sz * ddy + cz * ddx),
		sub_yz2 = cz * ddy - sz * ddx;
	float dx = cy * (sz * ddy + cz * ddx) - sy * ddz,
		dy = sx * sub_yz1 + cx * sub_yz2,
		dz = cx * sub_yz1 - sx * sub_yz2;
	(*ppoint)[0] = dx / dz * wcs;
	(*ppoint)[1] = dy / dz * wcs;
}

