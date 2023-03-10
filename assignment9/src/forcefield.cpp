#include "forcefield.h"

GravityForceField::GravityForceField(const Vec3f &g)
    : gravity(g)
{
}

Vec3f GravityForceField::getAcceleration(const Vec3f &position, float mass, float t) const
{
    return gravity;
}

ConstantForceField::ConstantForceField(const Vec3f &f)
    : force(f)
{
}

Vec3f ConstantForceField::getAcceleration(const Vec3f &position, float mass, float t) const
{
    Vec3f acc = force;
    acc /= mass;

    return acc;
}

RadialForceField::RadialForceField(float m)
    : magnitude(m)
{
}

Vec3f RadialForceField::getAcceleration(const Vec3f &position, float mass, float t) const
{
    Vec3f acc = -1.f * position;
    acc *= magnitude;
    acc /= mass;

    return acc;
}

VerticalForceField::VerticalForceField(float m)
    : magnitude(m)
{
}

Vec3f VerticalForceField::getAcceleration(const Vec3f &position, float mass, float t) const
{
    Vec3f acc(0.f, -position.y(), 0.f);
    acc *= magnitude;
    acc /= mass;

    return acc;
}

WindForceField::WindForceField(float m)
    :magnitude(m)
{
}

Vec3f WindForceField::getAcceleration(const Vec3f &position, float mass, float t) const
{
    int dir = roundf(position.y() * .5f);
    dir %= 2;
    dir = dir ? 1.f : -1.f;

    Vec3f acc(dir - position.x(), 0.f, 0.f);
    acc *= magnitude;
    acc /= mass;

    return acc;
}
