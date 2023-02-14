#include "camera.h"
#include "ray.h"
#include <float.h>

OrthographicCamera::OrthographicCamera(const Vec3f &_center, const Vec3f &_direction, const Vec3f &_up, float _size)
    :center(_center),direction(_direction),up(_up),size(_size)
{
    direction.Normalize();
    up.Normalize();

    Vec3f::Cross3(horizontal, direction, up);
    Vec3f::Cross3(up, horizontal, direction);

    horizontal.Normalize();
    up.Normalize();
}

Ray OrthographicCamera::generateRay(Vec2f point)
{
    float x = size * (point.x() - .5f);
    float y = size * (point.y() - .5f);

    Vec3f ray_origin = center + horizontal * x + up * y;
    return Ray(ray_origin, direction);
}

float OrthographicCamera::getTMin() const
{
    return -FLT_MAX;
}
