#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "object3d.h"
#include "matrix.h"

class BoundingBox;
BoundingBox getTransformedBoundingBox(const BoundingBox &box, const Matrix &transform);

class Transform : public Object3D
{
public:
    Transform(const Matrix &m, Object3D *o);
    virtual bool intersect(const Ray &r, Hit &h, float tmin) override;
    virtual void insertIntoGrid(Grid *g, Matrix *m) override;
    virtual void paint() override;

protected:
    Matrix worldTransform;
    Matrix normalTransform;
    Matrix objectTransform;
    Object3D *object;
};
#endif