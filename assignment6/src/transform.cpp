#include "transform.h"
#include "ray.h"
#include "hit.h"
#include "bounding_box.h"
#include <GL/gl.h>
#include <float.h>

BoundingBox getTransformedBoundingBox(const BoundingBox &box, const Matrix &transform)
{
    Vec3f box_min = box.getMin();
    Vec3f box_max = box.getMax();
    Vec3f center = (box_min + box_max) * .5f;
    Vec3f diag = (box_max - box_min) * .5f;

    Vec3f transformed_box_min = {FLT_MAX, FLT_MAX, FLT_MAX};
    Vec3f transformed_box_max = {-FLT_MAX, -FLT_MAX, -FLT_MAX};

    static Vec3f dir[8] = {
        {-1.f, -1.f, -1.f},
        {-1.f, -1.f, 1.f},
        {-1.f, 1.f, -1.f},
        {-1.f, 1.f, 1.f},
        {1.f, -1.f, -1.f},
        {1.f, -1.f, 1.f},
        {1.f, 1.f, -1.f},
        {1.f, 1.f, 1.f}};

    for (int i = 0; i < 8; ++i)
    {
        Vec3f diag_i = diag;
        diag_i.Scale(dir[i].x(), dir[i].y(), dir[i].z());
        Vec3f vert = center + diag_i;
        transform.Transform(vert);

        Vec3f::Min(transformed_box_min, transformed_box_min, vert);
        Vec3f::Max(transformed_box_max, transformed_box_max, vert);
    }

    return BoundingBox(transformed_box_min, transformed_box_max);
}

Transform::Transform(const Matrix &m, Object3D *o)
    : worldTransform(m), object(o)
{
    objectTransform = worldTransform;
    objectTransform.Inverse();

    normalTransform = objectTransform;
    normalTransform.Transpose();

    BoundingBox *obj_box = o->getBoundingBox();
    boundingBox = new BoundingBox(getTransformedBoundingBox(*obj_box, worldTransform));
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

void Transform::insertIntoGrid(Grid *g, Matrix *m)
{
    Matrix transform = (*m) * worldTransform;
    object->insertIntoGrid(g, &transform);
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
