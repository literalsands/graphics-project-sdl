#ifndef GP_REPR
#define GP_REPR

typedef int RGBColor[3];

typedef int Point1D; /* x + x*y + ... (Pre-Computed Point) */
typedef int Point2D[2]; /* [x, y] */
typedef int Point3D[3]; /* [x, y, z] */

typedef float RealPoint1D; /* x + x*y + ... (Pre-Computed Point) */
typedef float RealPoint2D[2]; /* [x, y] */
typedef float RealPoint3D[3]; /* [x, y, z] */

typedef Point2D Line2D[2];
typedef Point3D Line3D[2];

typedef RealPoint2D RealLine2D[2];
typedef RealPoint3D RealLine3D[2];

typedef Point2D * Surface2D;
typedef Point3D * Surface3D;

typedef Surface2D * Collection2D;
typedef Surface3D * Collection3D;

//typedef struct _Plane Plane;

//typedef Plane * Frame;

#endif
