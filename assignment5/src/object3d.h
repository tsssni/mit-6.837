#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_
#include "vectors.h"
#include <initializer_list>

class Ray;
class Hit;
class Material;
class BoundingBox;
class Grid;

void glFlatShade(const Vec3f &n, const Vec3f &v0, const Vec3f &v1, const Vec3f &v2);
void glFlatShade(const Vec3f &n, const Vec3f &v0, const Vec3f &v1, const Vec3f &v2, const Vec3f &v3);
void glGouroudShade(const Vec3f &n0, const Vec3f &v0, const Vec3f &n1, const Vec3f &v1, const Vec3f &n2, const Vec3f &v2);
void glGouroudShade(const Vec3f &n0, const Vec3f &v0, const Vec3f &n1, const Vec3f &v1, const Vec3f &n2, const Vec3f &v2, const Vec3f &n3, const Vec3f &v3);

class Object3D
{
public:
    virtual ~Object3D();
    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
    virtual void insertIntoGrid(Grid *g, Matrix *m);
    virtual void paint() = 0;

    BoundingBox *getBoundingBox();

protected:
    Material *mat = nullptr;
    BoundingBox *boundingBox = nullptr;
};

#endif