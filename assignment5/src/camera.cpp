#include "camera.h"
#include "ray.h"
#include "matrix.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <float.h>
#include <math.h>

Camera::Camera(const Vec3f &_center, const Vec3f &_direction, const Vec3f &_up)
    : center(_center), direction(_direction), up(_up)
{
    direction.Normalize();
    up.Normalize();
}

OrthographicCamera::OrthographicCamera(const Vec3f &_center, const Vec3f &_direction, const Vec3f &_up, float _size)
    : Camera(_center, _direction, _up), size(_size)
{
    
}

Ray OrthographicCamera::generateRay(Vec2f point)
{
    float x = size * (point.x() - .5f);
    float y = size * (point.y() - .5f);

    Vec3f horizontal;
    Vec3f::Cross3(horizontal, direction, up);
    horizontal.Normalize();

    Vec3f screenUp;
    Vec3f::Cross3(screenUp, horizontal, direction);

    Vec3f ray_origin = center + horizontal * x + screenUp * y;
    return Ray(ray_origin, direction);
}

float OrthographicCamera::getTMin() const
{
    return -FLT_MAX;
}

void OrthographicCamera::glInit(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w > h)
        glOrtho(-size / 2.0, size / 2.0, -size * (float)h / (float)w / 2.0, size * (float)h / (float)w / 2.0, 0.5, 40.0);
    else
        glOrtho(-size * (float)w / (float)h / 2.0, size * (float)w / (float)h / 2.0, -size / 2.0, size / 2.0, 0.5, 40.0);
}

// ====================================================================
// Place an orthographic camera within an OpenGL scene
// ====================================================================

void OrthographicCamera::glPlaceCamera(void)
{
    gluLookAt(center.x(), center.y(), center.z(),
              center.x() + direction.x(), center.y() + direction.y(), center.z() + direction.z(),
              up.x(), up.y(), up.z());
}

void OrthographicCamera::dollyCamera(float dist)
{
    center += direction * dist;

    // ===========================================
    // ASSIGNMENT 3: Fix any other affected values
    // ===========================================
}

void OrthographicCamera::truckCamera(float dx, float dy)
{
    Vec3f horizontal;
    Vec3f::Cross3(horizontal, direction, up);
    horizontal.Normalize();

    Vec3f screenUp;
    Vec3f::Cross3(screenUp, horizontal, direction);

    center += horizontal * dx + screenUp * dy;

    // ===========================================
    // ASSIGNMENT 3: Fix any other affected values
    // ===========================================
}

void OrthographicCamera::rotateCamera(float rx, float ry)
{
    Vec3f horizontal;
    Vec3f::Cross3(horizontal, direction, up);
    horizontal.Normalize();

    // Don't let the model flip upside-down (There is a singularity
    // at the poles when 'up' and 'direction' are aligned)
    float tiltAngle = acos(up.Dot3(direction));
    if (tiltAngle - ry > 3.13)
        ry = tiltAngle - 3.13;
    else if (tiltAngle - ry < 0.01)
        ry = tiltAngle - 0.01;

    Matrix rotMat = Matrix::MakeAxisRotation(up, rx);
    rotMat *= Matrix::MakeAxisRotation(horizontal, ry);

    rotMat.Transform(center);
    rotMat.TransformDirection(direction);

    // ===========================================
    // ASSIGNMENT 3: Fix any other affected values
    // ===========================================
}

PerspectiveCamera::PerspectiveCamera(const Vec3f &_center, const Vec3f &_direction, const Vec3f &_up, float _angle)
    : Camera(_center, _direction, _up), angle(_angle)
{
}

Ray PerspectiveCamera::generateRay(Vec2f point)
{
    float x = tanf((point.x() - .5f) * angle);
    float y = tanf((point.y() - .5f) * angle);

    Vec3f horizontal;
    Vec3f::Cross3(horizontal, direction, up);
    horizontal.Normalize();

    Vec3f screenUp;
    Vec3f::Cross3(screenUp, horizontal, direction);

    Vec3f ray_direction = direction + horizontal * x + screenUp * y;
    ray_direction.Normalize();
    return Ray(center, ray_direction);
}

float PerspectiveCamera::getTMin() const
{
    return 0.0f;
}

void PerspectiveCamera::glInit(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(angle * 180.0 / 3.14159, (float)w / float(h), 0.5, 40.0);
}

void PerspectiveCamera::glPlaceCamera(void)
{
    gluLookAt(center.x(), center.y(), center.z(),
              center.x() + direction.x(), center.y() + direction.y(), center.z() + direction.z(),
              up.x(), up.y(), up.z());
}

void PerspectiveCamera::dollyCamera(float dist)
{
    center += direction * dist;

    // ===========================================
    // ASSIGNMENT 3: Fix any other affected values
    // ===========================================
}

void PerspectiveCamera::truckCamera(float dx, float dy)
{
    Vec3f horizontal;
    Vec3f::Cross3(horizontal, direction, up);
    horizontal.Normalize();

    Vec3f screenUp;
    Vec3f::Cross3(screenUp, horizontal, direction);

    center += horizontal * dx + screenUp * dy;

    // ===========================================
    // ASSIGNMENT 3: Fix any other affected values
    // ===========================================
}

void PerspectiveCamera::rotateCamera(float rx, float ry)
{
    Vec3f horizontal;
    Vec3f::Cross3(horizontal, direction, up);
    horizontal.Normalize();

    // Don't let the model flip upside-down (There is a singularity
    // at the poles when 'up' and 'direction' are aligned)
    float tiltAngle = acos(up.Dot3(direction));
    if (tiltAngle - ry > 3.13)
        ry = tiltAngle - 3.13;
    else if (tiltAngle - ry < 0.01)
        ry = tiltAngle - 0.01;

    Matrix rotMat = Matrix::MakeAxisRotation(up, rx);
    rotMat *= Matrix::MakeAxisRotation(horizontal, ry);

    rotMat.Transform(center);
    rotMat.TransformDirection(direction);
    direction.Normalize();

    // ===========================================
    // ASSIGNMENT 3: Fix any other affected values
    // ===========================================
}
