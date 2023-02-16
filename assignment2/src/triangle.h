#include "object3d.h"
#include "vectors.h"

class Triangle : public Object3D
{
public:
    Triangle(const Vec3f &_a, const Vec3f &_b, const Vec3f &_c, Material *m);
    virtual bool intersect(const Ray &r, Hit &h, float tmin) override;

protected:
    Vec3f a;
    Vec3f b;
    Vec3f c;

    Vec3f e_a;
    Vec3f e_b;
    Vec3f e_c;

    Vec3f n_a;
    Vec3f n_b;
    Vec3f n_c;

    Vec3f normal;
    float d;
    float area;
};