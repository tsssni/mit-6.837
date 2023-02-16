#include "object3d.h"
#include "matrix.h"

class Transform : public Object3D
{
public:
    Transform(const Matrix &m, Object3D *o);
    virtual bool intersect(const Ray &r, Hit &h, float tmin) override;

protected:
    Matrix world_transform;
    Matrix normal_transform;
    Matrix object_transform;
    Object3D *object;
};