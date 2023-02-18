#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

class Ray;
class Hit;
class Material;
class Vec3f;

class Object3D
{
public:
    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
    virtual void paint() = 0;

    virtual void glFlatShade(const Vec3f &n, const Vec3f &v0, const Vec3f &v1, const Vec3f &v2);
    virtual void glFlatShade(const Vec3f &n, const Vec3f &v0, const Vec3f &v1, const Vec3f &v2, const Vec3f &v3);
    virtual void glGouroudShade(const Vec3f &n0, const Vec3f &v0, const Vec3f &n1, const Vec3f &v1, const Vec3f &n2, const Vec3f &v2);
    virtual void glGouroudShade(const Vec3f &n0, const Vec3f &v0, const Vec3f &n1, const Vec3f &v1, const Vec3f &n2, const Vec3f &v2, const Vec3f &n3, const Vec3f &v3);

protected:
    Material *mat;
};

#endif