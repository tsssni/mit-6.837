#include "transform.h"
#include "ray.h"
#include "hit.h"

Transform::Transform(const Matrix &m, Object3D *o)
    : world_transform(m), object(o)
{
    object_transform = world_transform;
    object_transform.Inverse();

    normal_transform = object_transform;
    normal_transform.Transpose();
}

bool Transform::intersect(const Ray &r, Hit &h, float tmin)
{
    Vec3f origin = r.getOrigin();
    Vec3f direction = r.getDirection();

    object_transform.Transform(origin);
    object_transform.TransformDirection(direction);
    direction.Normalize();

    Ray transformed_r(origin, direction);
    bool intersected = object->intersect(transformed_r, h, tmin);

    if (intersected)
    {
        Vec3f intersected_point = h.getIntersectionPoint();
        world_transform.Transform(intersected_point);
        float t = (intersected_point - r.getOrigin()).Dot3(r.getDirection());

        Vec3f normal = h.getNormal();
        normal_transform.TransformDirection(normal);
        normal.Normalize();

        h.set(t, h.getMaterial(), normal, r);
    }

    return intersected;
}
