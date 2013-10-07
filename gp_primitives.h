/* Austin New gp_primitives.h */
#define WIDTH 1280
#define HEIGHT 800

/* These are the two primitive types, Points, and Iterators, which are sets of points. */
typedef const int Point2D[2]; /* [x, y] */
typedef const int Point3D[3]; /* [x, y, z] */
typedef const int PPoint; /* x + x*y + ... (Pre-Computed Point) */

typedef struct _Iterator {
	// int ppoint;
	int * int_args;
	float * float_args;
	int (* iteration_function) (int*, float*); /* Returns 0 on finish. */
} Iterator;

/* These are contructors, iteration functions, and destructors for iterators. */
/* Line */
Iterator* Iterator_Line_BRZ(Point2D*, Point2D*);
int BRZ_Line_Iteration(int*, float*);

Iterator* Iterator_Line_DDA(Point2D*, Point2D*);
int DDA_Line_Iteration(int*, float*);

/* Circle */
Iterator* Iterator_Circle_BRZ(Point2D*, int);
int BRZ_Circle_Iteration(int*, float*);

/* This is the function that performs an iteration on an iterator. */
int Iterate(Iterator*);
void Delete_Iterator(Iterator*);
