#include "object3d.h"
#include "vectors.h"
#include <GL/gl.h>

void Object3D::glFlatShade(const Vec3f &n, const Vec3f &v0, const Vec3f &v1, const Vec3f &v2)
{
    glNormal3f(n.x(), n.y(), n.z());
    glVertex3f(v0.x(), v0.y(), v0.z());
    glVertex3f(v1.x(), v1.y(), v1.z());
    glVertex3f(v2.x(), v2.y(), v2.z());
}

void Object3D::glFlatShade(const Vec3f &n, const Vec3f &v0, const Vec3f &v1, const Vec3f &v2, const Vec3f &v3)
{
    glNormal3f(n.x(), n.y(), n.z());
    glVertex3f(v0.x(), v0.y(), v0.z());
    glVertex3f(v1.x(), v1.y(), v1.z());
    glVertex3f(v2.x(), v2.y(), v2.z());
    glVertex3f(v3.x(), v3.y(), v3.z());
}

void Object3D::glGouroudShade(const Vec3f &n0, const Vec3f &v0, const Vec3f &n1, const Vec3f &v1, const Vec3f &n2, const Vec3f &v2)
{
    glNormal3f(n0.x(), n0.y(), n0.z());
    glVertex3f(v0.x(), v0.y(), v0.z());
    glNormal3f(n1.x(), n1.y(), n1.z());
    glVertex3f(v1.x(), v1.y(), v1.z());
    glNormal3f(n2.x(), n2.y(), n2.z());
    glVertex3f(v2.x(), v2.y(), v2.z());
}

void Object3D::glGouroudShade(const Vec3f &n0, const Vec3f &v0, const Vec3f &n1, const Vec3f &v1, const Vec3f &n2, const Vec3f &v2, const Vec3f &n3, const Vec3f &v3)
{
    glNormal3f(n0.x(), n0.y(), n0.z());
    glVertex3f(v0.x(), v0.y(), v0.z());
    glNormal3f(n1.x(), n1.y(), n1.z());
    glVertex3f(v1.x(), v1.y(), v1.z());
    glNormal3f(n2.x(), n2.y(), n2.z());
    glVertex3f(v2.x(), v2.y(), v2.z());
    glNormal3f(n3.x(), n3.y(), n3.z());
    glVertex3f(v3.x(), v3.y(), v3.z());
}
