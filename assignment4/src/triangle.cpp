#include "triangle.h"
#include "ray.h"
#include "hit.h"
#include "matrix.h"
#include "material.h"
#include <GL/gl.h>
#include <math.h>
#include <stdio.h>

#define EPLISON 1e-6

Triangle::Triangle(const Vec3f &_a, const Vec3f &_b, const Vec3f &_c, Material *m)
    : a(_a), b(_b), c(_c),
      edgeA(_c - _b), edgeB(_a - _c), edgeC(_b - _a),
      normalA(_c - _b), normalB(_a - _c), normalC(_b - _a)
{
    mat = m;

    Vec3f::Cross3(normal, b - a, c - b);
    normal.Normalize();
    d = normal.Dot3(a);

    Matrix normal_rotation = Matrix::MakeAxisRotation(normal, M_PI * 0.5f);
    normal_rotation.Transform(normalA);
    normal_rotation.Transform(normalB);
    normal_rotation.Transform(normalC);

    area = normalA.Dot3(a - b);
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

        float u = normalA.Dot3(p - b) / area;
        float v = normalB.Dot3(p - c) / area;
        float w = 1.f - u - v;

        if (u >= -EPLISON && v >= -EPLISON && w >= -EPLISON)
        {
            h.set(t, mat, normal, r);
            intersected = true;
        }
    }

    return intersected;
}

void Triangle::paint()
{
    glBegin(GL_TRIANGLES);
    mat->glSetMaterial();
    glFlatShade(normal, a, b, c);
    glEnd();
}
