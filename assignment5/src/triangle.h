#include "object3d.h"
#include "vectors.h"

class Triangle : public Object3D
{
public:
    Triangle(const Vec3f &_a, const Vec3f &_b, const Vec3f &_c, Material *m);
    virtual bool intersect(const Ray &r, Hit &h, float tmin) override;
    virtual void insertIntoGrid(Grid *g, Matrix *m);
    virtual void paint() override;

protected:
    bool testPointInTriangle(const Vec3f &p);
    bool testBoundingBoxIntersectsWithTriangle(const BoundingBox &box);

    Vec3f a;
    Vec3f b;
    Vec3f c;

    Vec3f edgeA;
    Vec3f edgeB;
    Vec3f edgeC;

    Vec3f normalA;
    Vec3f normalB;
    Vec3f normalC;

    Vec3f normal;
    float d;
    float area;
};