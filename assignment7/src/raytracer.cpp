#include "raytracer.h"
#include "scene_parser.h"
#include "ray.h"
#include "hit.h"
#include "group.h"
#include "light.h"
#include "material.h"
#include "ray_tree.h"
#include "grid.h"
#include "raytracing_stats.h"
#include <float.h>
#include <math.h>
#define EPSILON 1e-3

extern bool gui;
extern bool shade_back;
extern bool grid;
extern bool visualize_grid;
extern bool stats;

Vec3f mirrorDirection(const Vec3f &normal, const Vec3f &incoming)
{
    Vec3f outcoming = -1.f * incoming;
    return 2 * (outcoming.Dot3(normal)) * normal - outcoming;
}

bool transmittedDirection(const Vec3f &normal, const Vec3f &incoming, float index_i, float index_t, Vec3f &transmitted)
{
    float cos_theta_i = -1.f * normal.Dot3(incoming);
    float sin_theta_i = sqrtf(1.f - cos_theta_i * cos_theta_i);
    float sin_theta_t = index_i / index_t * sin_theta_i;

    if (sin_theta_t < 1.f)
    {
        Vec3f m = incoming - normal.Dot3(incoming) * normal;
        m.Normalize();

        float cos_theta_t = sqrt(1.f - sin_theta_t * sin_theta_t);
        transmitted = -cos_theta_t * normal + sin_theta_t * m;
        transmitted.Normalize();
    }

    return sin_theta_t < 1.f;
}

bool RayCast(SceneParser *scene, const Ray &ray, Hit &hit, float tmin)
{
    return scene->getGroup()->intersect(ray, hit, tmin);
}

bool RayCastFast(Grid *grid, const Ray &ray, Hit &hit, float tmin)
{
    return grid->intersect(ray, hit, tmin);
}

RayTracer::RayTracer(SceneParser *s, Grid *g, int _max_bounces, float _cutoff_weight, bool _shadows)
    : scene(s), scene_grid(g), max_bounces(_max_bounces), cutoff_weight(_cutoff_weight), shadows(_shadows)
{
}

Vec3f RayTracer::traceRay(Ray &ray, float tmin, int bounces, float weight, float index_i, Hit &hit) const
{
    if (stats)
    {
        RayTracingStats::IncrementNumNonShadowRays();
    }

    Vec3f color = {0.f, 0.f, 0.f};

    if ((!grid && RayCast(scene, ray, hit, tmin)) ||
        (grid && RayCastFast(scene_grid, ray, hit, tmin)))
    {
        color += scene->getAmbientLight() * hit.getMaterial()->getDiffuseColor(hit.getIntersectionPoint());

        if (bounces < max_bounces && weight > cutoff_weight)
        {
            bool inside = ray.getDirection().Dot3(hit.getNormal()) > 0.f;
            Vec3f normal = inside ? -1.f * hit.getNormal() : hit.getNormal();

            Vec3f reflect_col = hit.getMaterial()->getReflectiveColor(hit.getIntersectionPoint());

            if (reflect_col.r() > 0.f || reflect_col.g() > 0.f || reflect_col.b() > 0.f)
            {
                Ray reflect_ray(hit.getIntersectionPoint(), mirrorDirection(normal, ray.getDirection()));
                Hit reflect_hit(FLT_MAX, nullptr, {0.f, 0.f, 0.f});
                color += reflect_col * traceRay(reflect_ray, EPSILON, bounces + 1, weight * reflect_col.Length(), index_i, reflect_hit);

                if (gui)
                {
                    RayTree::AddReflectedSegment(reflect_ray, 0.f, reflect_hit.getT());
                }
            }

            Vec3f refract_col = hit.getMaterial()->getTransparentColor(hit.getIntersectionPoint());
            float index_t = inside ? 1.f : hit.getMaterial()->getIndexOfRefraction(hit.getIntersectionPoint());
            Vec3f transmitted_dir;

            if ((refract_col.r() > 0.f || refract_col.g() > 0.f || refract_col.b() > 0.f) &&
                transmittedDirection(normal, ray.getDirection(), index_i, index_t, transmitted_dir))
            {
                Ray refract_ray(hit.getIntersectionPoint(), transmitted_dir);
                Hit refract_hit(FLT_MAX, nullptr, {0.f, 0.f, 0.f});
                color += refract_col * traceRay(refract_ray, EPSILON, bounces + 1, weight * refract_col.Length(), index_t, refract_hit);

                if (gui)
                {
                    RayTree::AddTransmittedSegment(refract_ray, 0.f, refract_hit.getT());
                }
            }
        }

        for (int i = 0; i < scene->getNumLights(); ++i)
        {
            Vec3f dir;
            Vec3f col;
            float dist;
            scene->getLight(i)->getIllumination(hit.getIntersectionPoint(), dir, col, dist);

            Ray to_light_ray(hit.getIntersectionPoint(), dir);
            Hit to_light_hit(FLT_MAX, nullptr, {0.f, 0.f, 0.f});

            bool shadow_intersected = false;

            if (shadows)
            {
                shadow_intersected = (!grid) ? RayCast(scene, to_light_ray, to_light_hit, EPSILON) : RayCastFast(scene_grid, to_light_ray, to_light_hit, EPSILON);

                if (gui)
                {
                    RayTree::AddShadowSegment(to_light_ray, 0.f, fmin(to_light_hit.getT(), dist));
                }

                if (stats)
                {
                    RayTracingStats::IncrementNumShadowRays();
                }
            }

            if (!shadow_intersected || (shadow_intersected && to_light_hit.getT() >= dist))
            {
                color += hit.getMaterial()->Shade(ray, hit, dir, col);
            }
        }
    }
    else
    {
        color = scene->getBackgroundColor();
    }

    return color;
}
