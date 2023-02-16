#include "plane.h"
#include "ray.h"
#include "hit.h"

Plane::Plane(const Vec3f &_normal, float _d, Material *m)
    : normal(_normal), d(_d)
{
    normal.Normalize();
    mat = m;
}

bool Plane::intersect(const Ray &r, Hit &h, float tmin)
{
    float t1 = normal.Dot3(r.getDirection());
    float t2 = normal.Dot3(r.getOrigin());
    float t = (d - t2) / t1;

    bool intersected = false;
    if (t > tmin)
    {
        h.set(t, mat, normal, r);
        intersected = true;
    }

    return intersected;
}
