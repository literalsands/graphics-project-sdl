#ifndef GP_TRANS
#define GP_TRANS

typedef float TransformMatrix2D[3][3];
// This includes the Affine Transformations (Translate, etc.)
// typedef float TransformMatrix2D[2][2];
typedef float TransformMatrix3D[4][4];
// This includes the Affine Transformations (Translate, etc.)
// typedef float TransformMatrix3D[3][3];

void TranslateTransformMatrix2D(TransformMatrix2D*, float, float);
void ScaleTransformMatrix2D(TransformMatrix2D*, float, float);
void RotateTransformMatrix2D(TransformMatrix2D*, float);
void PivotTranformMatrix2D(TransformMatrix2D*, int, int);
void ConcatenateTransformMatrix2D(TransformMatrix2D*, TransformMatrix2D*);

void Point2DTransformation(RealPoint2D*, TransformMatrix2D*);

void TranslateTransformMatrix3D(TransformMatrix3D*, float, float);
void ScaleTransformMatrix3D(TransformMatrix3D*, float, float);
void RotateTransformMatrix3D(TransformMatrix3D*, float);
void PivotTranformMatrix3D(TransformMatrix3D*, int, int);
void ConcatenateTransformMatrix3D(TransformMatrix3D*, TransformMatrix3D*);

void Point3DTransformation(RealPoint3D*, TransformMatrix3D*);

#endif
