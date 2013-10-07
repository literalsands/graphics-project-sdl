/* Austin New gp_perform.h */
#include "gp_primitives.h"

typedef struct _RGBColor {
	Uint8 red, green, blue;
} RGBColor;

typedef struct _Routine {
	Iterator * iterator;
	int iterations;
	int (* plot) (Iterator*, int);
	RGBColor (* coloration) (Iterator*, int);
} Routine;

/*int Perform(Routine**);*/
