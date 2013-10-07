#include "gp_repr.h"
#include "gp_trans.h"
#include <math.h>

void TranslateTransformMatrix2D(TransformMatrix2D* c, float sx, float sy) {

	/*	* * * * * 
		1	0	sx
		0	1	sy
		0	0	1				
	 * * * * * */
	 
	(*c)[0][0] += (*c)[0][2] * sx;
	(*c)[0][1] += (*c)[0][2] * sy;
	(*c)[1][0] += (*c)[1][2] * sx;
	(*c)[1][1] += (*c)[1][2] * sy;
	(*c)[2][0] += (*c)[2][2] * sx;
	(*c)[2][1] += (*c)[2][2] * sy;
}

void ScaleTransformMatrix2D(TransformMatrix2D* c, float sx, float sy) {

	/*	* * * * * 
		sx	0	0
		0	sy	0
		0	0	1				
	 * * * * * */
	 
	(*c)[0][0] *= sx;
	(*c)[0][1] *= sy;
	(*c)[1][0] *= sx;
	(*c)[1][1] *= sy;
	(*c)[2][0] *= sx;
	(*c)[2][1] *= sy;
}

void RotateTransformMatrix2D(TransformMatrix2D* c, float t) {

	/*	* * * * * Clock-wise Rotation
		c	s	0
		-s	c	0
		0	0	1				
	 * * * * * */
	 
	// cos(theta), sin(theta)
	float ct = cosf(t), st = -sinf(t); 
	
	// Copy what would be changed during computation;
	float zz = (*c)[0][0], // z ~ 0, w ~ 1, t ~ 2
		zw = (*c)[0][1],
		wz = (*c)[1][0],
		ww = (*c)[1][1],
		tz = (*c)[2][0],
		tw = (*c)[2][1];
	
	(*c)[0][0] = zz * ct + zw * st;
	(*c)[0][1] = zz * -st + zw * ct;
	(*c)[1][0] = wz * ct + ww * st;
	(*c)[1][1] = wz * -st + ww * ct;
	(*c)[2][0] = tz * ct + tw * st;
	(*c)[2][1] = tz * -st + tw * ct;
}

void Point2DTransformation(RealPoint2D* line, TransformMatrix2D* c) {
	// Copy what would be changed during computation;
	int z = *(line)[0], w = (*line)[1];

	(*line)[0] = roundf(z * (*c)[0][0] + w * (*c)[1][0] + (*c)[2][0]);
	(*line)[1] = roundf(z * (*c)[0][1] + w * (*c)[1][1] + (*c)[2][1]);
}

/* TODO 3DTransformation Matrices */