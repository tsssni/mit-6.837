#include "triangle.h"
#include "ray.h"
#include "hit.h"
#include "matrix.h"
#include "material.h"
#include "vectors.h"
#include "bounding_box.h"
#include "grid.h"
#include <GL/gl.h>
#include <math.h>
#include <stdio.h>
#include <float.h>
#include <initializer_list>

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

    Vec3f box_min = {FLT_MAX, FLT_MAX, FLT_MAX};
    Vec3f box_max = {-FLT_MAX, -FLT_MAX, -FLT_MAX};

    Vec3f::Min(box_min, box_min, a);
    Vec3f::Min(box_min, box_min, b);
    Vec3f::Min(box_min, box_min, c);

    Vec3f::Max(box_max, box_max, a);
    Vec3f::Max(box_max, box_max, b);
    Vec3f::Max(box_max, box_max, c);

    boundingBox = new BoundingBox(box_min, box_max);
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

        if (testPointInTriangle(p))
        {
            h.set(t, mat, normal, r);
            intersected = true;
        }
    }

    return intersected;
}

void Triangle::insertIntoGrid(Grid *g, Matrix *m)
{
    for (int i = 0; i < g->getX(); ++i)
    {
        for (int j = 0; j < g->getY(); ++j)
        {
            for (int k = 0; k < g->getZ(); ++k)
            {
                BoundingBox box = g->getCellBox(i, j, k);

                if (testBoundingBoxIntersectsWithTriangle(box))
                {
                    g->setCell(i, j, k, this);
                }
            }
        }
    }
}

void Triangle::paint()
{
    glBegin(GL_TRIANGLES);
    mat->glSetMaterial();
    glFlatShade(normal, a, b, c);
    glEnd();
}

bool Triangle::testPointInTriangle(const Vec3f &p)
{
    float u = normalA.Dot3(p - b) / area;
    float v = normalB.Dot3(p - c) / area;
    float w = 1.f - u - v;

    return u >= -EPLISON && v >= -EPLISON && w >= -EPLISON;
}

bool Triangle::testBoundingBoxIntersectsWithTriangle(const BoundingBox &box)
{
    // seperating axis theorem

    Vec3f box_min = box.getMin();
    Vec3f box_max = box.getMax();
    Vec3f center = (box_min + box_max) * .5f;

    Vec3f vert[3] = {a, b, c};
    Vec3f edge[3] = {edgeA, edgeB, edgeC};

    box_min -= center;
    box_max -= center;
    vert[0] -= center;
    vert[1] -= center;
    vert[2] -= center;

    // 3 tests on bounding-box normals

    Vec3f box_normal[3] = {{1.f, 0.f, 0.f}, {0.f, 1.f, 0.f}, {0.f, 0.f, 1.f}};
    for (int i = 0; i < 3; ++i)
    {
        float box_p[3] = {vert[0][i], vert[1][i], vert[2][i]};
        float p_min = fmin(box_p[0], fmin(box_p[1], box_p[2]));
        float p_max = fmax(box_p[0], fmax(box_p[1], box_p[2]));

        if (p_min > box_max[i] + EPLISON || p_max < box_min[i] - EPLISON)
        {
            return false;
        }
    }

    // 1 test on triangle normal via plane-aabb test

    float s = center.Dot3(normal) - d;
    float e = box_max.Dot3(Vec3f(fabs(normal.x()), fabs(normal.y()), fabs(normal.z())));

    if (s + e < 0.f || s - e > 0.f)
    {
        return false;
    }

    // 9 tests on cross product of triangle edges with aabb normals

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            Vec3f cross_a;
            Vec3f::Cross3(cross_a, box_normal[i], edge[j]);
            cross_a.Normalize();

            float t_p[3] = {cross_a.Dot3(edge[0]), cross_a.Dot3(edge[1]), cross_a.Dot3(edge[2])};
            float p_min = fmin(t_p[0], fmin(t_p[1], t_p[2]));
            float p_max = fmax(t_p[0], fmax(t_p[1], t_p[2]));

            float r = box_max.Dot3(Vec3f(fabs(cross_a.x()), fabs(cross_a.y()), fabs(cross_a.z())));
            if (p_min > r || p_max < -r)
            {
                return false;
            }
        }
    }

    return true;
}
