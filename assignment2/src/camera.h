#ifndef _CAMERA_H_
#define _CAMERA_H_
#include "vectors.h"

class Ray;

class Camera
{
public:
    Camera(const Vec3f &_center, const Vec3f &_direction, const Vec3f &_up);
    virtual Ray generateRay(Vec2f point) = 0;
    virtual float getTMin() const = 0;

protected:
    Vec3f center;
    Vec3f direction;
    Vec3f up;
    Vec3f horizontal;
};

class OrthographicCamera : public Camera
{
public:
    OrthographicCamera(const Vec3f &_center, const Vec3f &_direction, const Vec3f &_up, float _size);
    virtual Ray generateRay(Vec2f point) override;
    virtual float getTMin() const override;

protected:
    float size;
};

class PerspectiveCamera : public Camera
{
public:
    PerspectiveCamera(const Vec3f &_center, const Vec3f &_direction, const Vec3f &_up, float _angle);
    virtual Ray generateRay(Vec2f point) override;
    virtual float getTMin() const override;
protected:
    float angle;
};

#endif