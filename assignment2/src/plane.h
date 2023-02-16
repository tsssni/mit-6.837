#include "object3d.h"
#include "vectors.h"

class Plane : public Object3D
{
public:
    Plane(const Vec3f &_normal, float _d, Material *m);
    virtual bool intersect(const Ray &r, Hit &h, float tmin) override;

protected:
    Vec3f normal;
    float d;
};