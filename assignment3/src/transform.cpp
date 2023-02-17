#include "transform.h"
#include "ray.h"
#include "hit.h"
#include <GL/gl.h>

Transform::Transform(const Matrix &m, Object3D *o)
    : worldTransform(m), object(o)
{
    objectTransform = worldTransform;
    objectTransform.Inverse();

    normalTransform = objectTransform;
    normalTransform.Transpose();
}

bool Transform::intersect(const Ray &r, Hit &h, float tmin)
{
    Vec3f origin = r.getOrigin();
    Vec3f direction = r.getDirection();

    objectTransform.Transform(origin);
    objectTransform.TransformDirection(direction);
    direction.Normalize();

    Ray transformed_r(origin, direction);
    bool intersected = object->intersect(transformed_r, h, tmin);

    if (intersected)
    {
        Vec3f intersected_point = h.getIntersectionPoint();
        worldTransform.Transform(intersected_point);
        float t = (intersected_point - r.getOrigin()).Dot3(r.getDirection());

        Vec3f normal = h.getNormal();
        normalTransform.TransformDirection(normal);
        normal.Normalize();

        h.set(t, h.getMaterial(), normal, r);
    }

    return intersected;
}

void Transform::paint()
{
    glPushMatrix();
    GLfloat *gl_mat = worldTransform.glGet();
    glMultMatrixf(gl_mat);
    delete[] gl_mat;

    object->paint();
    glPopMatrix();
}
