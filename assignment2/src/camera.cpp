#include "camera.h"
#include "ray.h"
#include <float.h>
#include <math.h>

Camera::Camera(const Vec3f &_center, const Vec3f &_direction, const Vec3f &_up)
    :center(_center),direction(_direction),up(_up)
{
    direction.Normalize();
    up.Normalize();

    Vec3f::Cross3(horizontal, direction, up);
    horizontal.Normalize();

    Vec3f::Cross3(up, horizontal, direction);
    up.Normalize();
}

OrthographicCamera::OrthographicCamera(const Vec3f &_center, const Vec3f &_direction, const Vec3f &_up, float _size)
    :Camera(_center, _direction, _up), size(_size)
{
    
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

PerspectiveCamera::PerspectiveCamera(const Vec3f &_center, const Vec3f &_direction, const Vec3f &_up, float _angle)
    : Camera(_center, _direction, _up), angle(_angle)
{
}

Ray PerspectiveCamera::generateRay(Vec2f point)
{
    float x = tanf((point.x() - .5f) * angle);
    float y = tanf((point.y() - .5f) * angle);

    Vec3f ray_direction = direction + horizontal * x + up * y;
    ray_direction.Normalize();
    return Ray(center, ray_direction);
}

float PerspectiveCamera::getTMin() const
{
    return 0.0f;
}
