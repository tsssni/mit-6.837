#include "group.h"
#include "ray.h"
#include "hit.h"
#include <assert.h>
#include <float.h>

Group::Group(int num)
    : objects(new Object3D*[num]), num_objects(num)
{
}

Group::~Group()
{
    if(objects)
    {
        delete[] objects;
    }
}

bool Group::intersect(const Ray &r, Hit &h, float tmin)
{
    bool intersected = false;
    Hit temp_h(FLT_MAX, nullptr, {0.f, 0.f, 0.f});
    
    for (int i = 0; i < num_objects; ++i)
    {
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
    assert(index < num_objects);
    objects[index] = object;
}