#include "plane.h"
#include "ray.h"
#include "hit.h"
#include "material.h"
#include <GL/gl.h>

Plane::Plane(const Vec3f &_normal, float _d, Material *m)
    : normal(_normal), d(_d)
{
    normal.Normalize();
    mat = m;
    origin = d * normal;

    if (normal == Vec3f(1.f, 0.f, 0.f))
    {
        Vec3f::Cross3(b1, {0.f, 1.f, 0.f}, normal);
    }
    else
    {
        Vec3f::Cross3(b1, {1.f, 0.f, 0.f}, normal);
    }

    Vec3f::Cross3(b2, normal, b1);
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

void Plane::paint()
{
    static float border = 1e6 - 1;

    Vec3f v0 = origin - border * b2 - border * b1;
    Vec3f v1 = origin - border * b2 + border * b1;
    Vec3f v2 = origin + border * b2 + border * b1;
    Vec3f v3 = origin + border * b2 - border * b1;

    glBegin(GL_QUADS);
    mat->glSetMaterial();
    glFlatShade(normal, v0, v1, v2, v3);
    glEnd();
}
