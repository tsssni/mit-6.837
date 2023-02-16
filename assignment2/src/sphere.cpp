#include "sphere.h"
#include "ray.h"
#include "hit.h"

Sphere::Sphere(const Vec3f& _center, float _radius, Material *m)
    :center(_center), radius(_radius)
{
    mat = m;
}

bool Sphere::intersect(const Ray &r, Hit &h, float tmin)
{
    const Vec3f &dir = r.getDirection();
    const Vec3f &origin = r.getOrigin();
    Vec3f tmp;
    Vec3f::Sub(tmp, origin, center);

    float a = dir.Dot3(dir);
    float b = 2 * tmp.Dot3(dir);
    float c = tmp.Dot3(tmp) - radius * radius;

    float delta = b * b - 4.f * a * c;
    bool intersected = false;

    if (delta >= 0)
    {
        delta = sqrtf(delta);
        float t1 = (-b - delta) / (2.f * a);
        float t2 = (-b + delta) / (2.f * a);

        if (t1 > tmin || t2 > tmin)
        {
            float t = t1 > tmin ? t1 : t2;
            Vec3f p = r.pointAtParameter(t);
            Vec3f n = p - center;
            n.Normalize();

            h.set(t, mat, n, r);
            intersected = true;
        }
    }

    return intersected;
}