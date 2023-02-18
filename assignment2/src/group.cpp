#include "group.h"
#include "ray.h"
#include "hit.h"
#include <assert.h>
#include <float.h>

Group::Group(int num)
    : objects(new Object3D *[num]), numObjects(num)
{
}

Group::~Group()
{
    if (objects)
    {
        delete[] objects;
    }
}

bool Group::intersect(const Ray &r, Hit &h, float tmin)
{
    bool intersected = false;

    for (int i = 0; i < numObjects; ++i)
    {
        Hit temp_h(FLT_MAX, nullptr, {0.f, 0.f, 0.f});

        if (objects[i]->intersect(r, temp_h, tmin) && temp_h.getT() < h.getT())
        {
            intersected = true;
            h = temp_h;
        }
    }

    return intersected;
}

void Group::addObject(int index, Object3D *object)
{
    assert(index < numObjects);
    objects[index] = object;
}