#ifndef _SPHERE_H_
#define _SPHERE_H_
#include "object3d.h"
#include "vectors.h"

class Grid;

class Sphere : public Object3D
{
public:
    Sphere(const Vec3f &_center, float _radius, Material *m);
    ~Sphere();

    virtual bool intersect(const Ray &r, Hit &h, float tmin) override;
    virtual void insertIntoGrid(Grid *g, Matrix *m) override;
    virtual void paint() override;

protected:
    Vec3f center;
    float radius;

    Vec3f northPole;
    Vec3f southPole;
    Vec3f *vertex;

    Vec3f northNormal;
    Vec3f southNormal;
    Vec3f *normal;
};

#endif