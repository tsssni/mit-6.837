#include "group.h"
#include "ray.h"
#include "hit.h"
#include "bounding_box.h"
#include "plane.h"
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

void Group::insertIntoGrid(Grid *g, Matrix *m)
{
    for (int i = 0; i < numObjects; ++i)
    {
        objects[i]->insertIntoGrid(g, m);
    }
}

void Group::paint()
{
    for (int i = 0; i < numObjects; ++i)
    {
        objects[i]->paint();
    }
}

void Group::addObject(int index, Object3D *object)
{
    assert(index < numObjects);
    objects[index] = object;

    Plane *p = dynamic_cast<Plane *>(object);

    if (!p)
    {
        if (!boundingBox)
        {
            boundingBox = new BoundingBox(*object->getBoundingBox());
        }
        else
        {   
            boundingBox->Extend(object->getBoundingBox());
        }
    }
}