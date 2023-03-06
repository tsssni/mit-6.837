#ifndef _PLANE_H_
#define _PLANE_H_

#include "object3d.h"
#include "vectors.h"

class Plane : public Object3D
{
public:
    Plane(const Vec3f &_normal, float _d, Material *m);
    virtual bool intersect(const Ray &r, Hit &h, float tmin) override;
    virtual void paint() override;

protected:
    Vec3f normal;
    float d;

    Vec3f origin;
    Vec3f b1;
    Vec3f b2;
};
#endif