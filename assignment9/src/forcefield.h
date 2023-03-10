#ifndef _FORCEFIELD_H_
#define _FORCEFIELD_H_

#include "vectors.h"

class ForceField
{
public:
    virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const = 0;
};

class GravityForceField : public ForceField
{
public:
    GravityForceField(const Vec3f &g);
    virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const;

protected:
    Vec3f gravity;
};

class ConstantForceField : public ForceField
{
public:
    ConstantForceField(const Vec3f &f);
    virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const;

protected:
    Vec3f force;
};

class RadialForceField : public ForceField
{
public:
    RadialForceField(float m);
    virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const;

protected:
    float magnitude;
};

class VerticalForceField : public ForceField
{
public:
    VerticalForceField(float m);
    virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const;

protected:
    float magnitude;
};

class WindForceField : public ForceField
{
public:
    WindForceField(float m);
    virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const;

protected:
    float magnitude;
};
#endif