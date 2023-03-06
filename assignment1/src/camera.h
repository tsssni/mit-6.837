#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "vectors.h"

class Ray;

class Camera
{
public:
    virtual Ray generateRay(Vec2f point) = 0;
    virtual float getTMin() const = 0;
};

class OrthographicCamera : public Camera
{
public:
    OrthographicCamera(const Vec3f &_center, const Vec3f &_direction, const Vec3f &_up, float _size);
    virtual Ray generateRay(Vec2f point) override;
    virtual float getTMin() const override;

protected:
    Vec3f center;
    Vec3f direction;
    Vec3f up;
    Vec3f horizontal;
    float size;
};

#endif