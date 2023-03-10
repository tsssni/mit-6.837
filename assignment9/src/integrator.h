#ifndef _INTEGRATOR_H_
#define _INTEGRATOR_H_

#include "vectors.h"

class Particle;
class ForceField;

class Integrator
{
public:
    virtual void Update(Particle *particle, ForceField *forcefield, float t, float dt) = 0;
    const Vec3f& getColor();

protected:
    Vec3f color;
};

class EulerIntegrator : public Integrator
{
public:
    EulerIntegrator();
    virtual void Update(Particle *particle, ForceField *forcefield, float t, float dt);
};

class MidpointIntegrator : public Integrator
{
public:
    MidpointIntegrator();
    virtual void Update(Particle *particle, ForceField *forcefield, float t, float dt);
};

class RungeKuttaIntegrator : public Integrator
{
public:
    RungeKuttaIntegrator();
    virtual void Update(Particle *particle, ForceField *forcefield, float t, float dt);
};
#endif