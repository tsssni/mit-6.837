#ifndef _GROUP_H_
#define _GROUP_H_
#include "object3d.h"

class Group : public Object3D
{
public:
    Group(int num);
    ~Group();

    virtual bool intersect(const Ray &r, Hit &h, float tmin) override;
    void addObject(int index, Object3D *object);

protected:
    Object3D** objects;
    int num_objects;
};

#endif