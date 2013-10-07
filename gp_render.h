#ifndef GP_RENDER
#define GP_RENDER

/* void draw_SDL_Surface(SDL_Surface*, Routine**); Drawing is setting the screen. */
void plot_PPoint(SDL_Surface*, int*, RGBColor*); /* Plot Pre-computed Y Coordinate. */
void plot_Point(SDL_Surface*, Point2D*, RGBColor*); /* Plot Point where Y is Y * WIDTH. */

typedef Point2D ViewPort[2]; // Upper Left Corner, Lower Right Corner; Must be in order.
void setViewPort(SDL_Surface*, ViewPort*, int, int, int, int);
void ClipLine2DAndRender(SDL_Surface*, ViewPort*, Line2D*, RGBColor*); // Renders a 2DLine to the ViewPort

void PerspectiveProjection3D(RealPoint2D*, SDL_Surface*, RealPoint3D*, RealPoint3D*, float, float);
void setViewPoint(SDL_Surface*, ViewPort*, Point3D*);
void PerspectiveProject3DAndRender(SDL_Surface*, ViewPort*, Line3D*, RGBColor*); // Renders a 3DLine to the ViewPort
// Then passes line to ClipLine2DAndRender.

typedef struct _Iterator Iterator;
int Iterate(Iterator*);
void Delete_Iterator(Iterator*);

Iterator* Iterator_Line2D(SDL_Surface* , Point2D*, Point2D*);
int Line2D_Iteration(int*, float*);

/* TODO Make the Iterator concept more full and univerally implementable. */
struct _Iterator {
	int * int_args;
	float * float_args;
	int (* iteration_function) (int*, float*); /* Returns 0 on finish. */
};

#endif
