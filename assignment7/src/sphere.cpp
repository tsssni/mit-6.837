#include "sphere.h"
#include "ray.h"
#include "hit.h"
#include "material.h"
#include "bounding_box.h"
#include "grid.h"
#include "matrix.h"
#include "transform.h"
#include "raytracing_stats.h"
#include <GL/gl.h>
#include <math.h>
#include <float.h>

extern int tess_phi;
extern int tess_theta;
extern bool gouraud;
extern bool stats;
extern bool shade_back;

Sphere::Sphere(const Vec3f &_center, float _radius, Material *m)
    : center(_center), radius(_radius)
{
    mat = m;
    northPole = center + Vec3f(0.f, radius, 0.f);
    southPole = center + Vec3f(0.f, -radius, 0.f);
    vertex = new Vec3f[(tess_phi - 1) * tess_theta];

    if (gouraud)
    {
        northNormal = {0.f, 1.f, 0.f};
        southNormal = {0.f, -1.f, 0.f};

        normal = new Vec3f[(tess_phi - 1) * tess_theta];
    }

    for (int i_phi = 1; i_phi < tess_phi; ++i_phi)
    {
        for (int i_theta = 0; i_theta < tess_theta; ++i_theta)
        {
            float cos_phi = cosf(i_phi * M_PI / tess_phi);
            float sin_phi = sinf(i_phi * M_PI / tess_phi);
            float cos_theta = cosf(i_theta * 2.f * M_PI / tess_theta);
            float sin_theta = sinf(i_theta * 2.f * M_PI / tess_theta);

            int i = (i_phi - 1) * tess_theta + i_theta;
            vertex[i] = center +
                        Vec3f(radius * sin_phi * sin_theta,
                              radius * cos_phi,
                              radius * sin_phi * cos_theta);

            if (gouraud)
            {
                normal[i] = vertex[i] - center;
                normal[i].Normalize();
            }
        }
    }

    boundingBox = new BoundingBox({center - Vec3f{radius, radius, radius}}, {center + Vec3f{radius, radius, radius}});
}

Sphere::~Sphere()
{
    if (vertex)
    {
        delete[] vertex;
    }

    if (normal)
    {
        delete[] normal;
    }
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

            if (stats)
            {
                RayTracingStats::IncrementNumIntersections();
            }
        }
    }

    return intersected;
}

void Sphere::insertIntoGrid(Grid *g, Matrix *m)
{
    Object3D *transformedObject = g->addTransformedObject(this, *m);
    Matrix objSpaceTransform = *m;
    objSpaceTransform.Inverse();

    for (int i = 0; i < g->getX(); ++i)
    {
        for (int j = 0; j < g->getY(); ++j)
        {
            for (int k = 0; k < g->getZ(); ++k)
            {
                BoundingBox cell_box = getTransformedBoundingBox(g->getCellBox(i, j, k), objSpaceTransform);
                Vec3f cell_box_min = cell_box.getMin();
                Vec3f cell_box_max = cell_box.getMax();
                Vec3f cell_box_center = (cell_box_min + cell_box_max) * .5f;
                float cell_diag = .5f * (cell_box_max - cell_box_min).Length();

                if ((cell_box_center - center).Length() < radius + cell_diag)
                {
                    g->setCell(i, j, k, transformedObject);
                }
            }
        }
    }
}

void Sphere::paint()
{
    glBegin(GL_TRIANGLES);
    mat->glSetMaterial();

    int north_pole_start = 0;
    int south_pole_start = (tess_phi - 2) * tess_theta;

    for (int i_theta = 0; i_theta < tess_theta; ++i_theta)
    {
        int i_theta_next = (i_theta + 1) % tess_theta;

        int north_i = north_pole_start + i_theta;
        int north_i_next = north_pole_start + i_theta_next;
        const Vec3f &vn0 = vertex[north_i];
        const Vec3f &vn1 = vertex[north_i_next];

        if (gouraud)
        {
            const Vec3f &nn0 = normal[north_i];
            const Vec3f &nn1 = normal[north_i_next];
            glGouroudShade(northNormal, northPole, nn0, vn0, nn1, vn1);
        }
        else
        {
            Vec3f w1 = vn0 - northPole;
            Vec3f w2 = vn1 - vn0;

            Vec3f normal;
            Vec3f::Cross3(normal, w1, w2);
            normal.Normalize();

            glFlatShade(normal, northPole, vn0, vn1);
        }

        int south_i = south_pole_start + i_theta;
        int south_i_next = south_pole_start + i_theta_next;
        const Vec3f &vs0 = vertex[south_i];
        const Vec3f &vs1 = vertex[south_i_next];

        if (gouraud)
        {
            const Vec3f &ns0 = normal[south_i];
            const Vec3f &ns1 = normal[south_i_next];
            glGouroudShade(southNormal, southPole, ns1, vs1, ns0, vs0);
        }
        else
        {
            Vec3f w1 = vs1 - southPole;
            Vec3f w2 = vs0 - vs1;

            Vec3f normal;
            Vec3f::Cross3(normal, w1, w2);
            normal.Normalize();

            glFlatShade(normal, southPole, vs1, vs0);
        }
    }
    glEnd();

    glBegin(GL_QUADS);
    for (int i_phi = 1; i_phi < tess_phi - 1; ++i_phi)
    {
        int start = (i_phi - 1) * tess_theta;
        int start_next = i_phi * tess_theta;

        for (int i_theta = 0; i_theta < tess_theta; ++i_theta)
        {
            int i_theta_next = (i_theta + 1) % tess_theta;
            const Vec3f &v0 = vertex[start + i_theta_next];
            const Vec3f &v1 = vertex[start + i_theta];
            const Vec3f &v2 = vertex[start_next + i_theta];
            const Vec3f &v3 = vertex[start_next + i_theta_next];

            if (gouraud)
            {
                const Vec3f &n0 = normal[start + i_theta_next];
                const Vec3f &n1 = normal[start + i_theta];
                const Vec3f &n2 = normal[start_next + i_theta];
                const Vec3f &n3 = normal[start_next + i_theta_next];

                glGouroudShade(n0, v0, n1, v1, n2, v2, n3, v3);
            }
            else
            {
                Vec3f w1 = v3 - v0;
                Vec3f w2 = v1 - v0;

                Vec3f n;
                Vec3f::Cross3(n, w1, w2);
                n.Normalize();

                glFlatShade(n, v0, v1, v2, v3);
            }
        }
    }
    glEnd();
}
