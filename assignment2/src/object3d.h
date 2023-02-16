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

protected:
    Material* mat;
};

#endif