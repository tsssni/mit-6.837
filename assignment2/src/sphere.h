#ifndef _SPHERE_H_
#define _SPHERE_H_
#include "object3d.h"
#include "vectors.h"

class Sphere : public Object3D
{
public:
    Sphere(const Vec3f& _center, float _radius, Material *m);
    virtual bool intersect(const Ray &r, Hit &h, float tmin) override;

protected:
    Vec3f center;
    float radius;
};

#endif