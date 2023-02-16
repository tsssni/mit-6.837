#include "triangle.h"
#include "ray.h"
#include "hit.h"
#include "matrix.h"
#include <math.h>
#include <stdio.h>

#define EPLISON 1e-6

Triangle::Triangle(const Vec3f &_a, const Vec3f &_b, const Vec3f &_c, Material *m)
    : a(_a), b(_b), c(_c),
      e_a(_c - _b), e_b(_a - _c), e_c(_b - _a),
      n_a(_c - _b), n_b(_a - _c), n_c(_b - _a)
{
    mat = m;

    Vec3f::Cross3(normal, b - a, c - b);
    normal.Normalize();
    d = normal.Dot3(a);

    Matrix normal_rotation = Matrix::MakeAxisRotation(normal, M_PI * 0.5f);
    normal_rotation.Transform(n_a);
    normal_rotation.Transform(n_b);
    normal_rotation.Transform(n_c);

    area = n_a.Dot3(a - b);
}

bool Triangle::intersect(const Ray &r, Hit &h, float tmin)
{
    float t1 = normal.Dot3(r.getDirection());
    float t2 = normal.Dot3(r.getOrigin());
    float t = (d - t2) / t1;

    bool intersected = false;
    if (t > tmin)
    {
        Vec3f p = r.pointAtParameter(t);

        float u = n_a.Dot3(p - b) / area;
        float v = n_b.Dot3(p - c) / area;
        float w = 1.f - u - v;

        if (u >= -EPLISON && v >= -EPLISON && w >= -EPLISON)
        {
            h.set(t, mat, normal, r);
            intersected = true;
        }
    }

    return intersected;
}
