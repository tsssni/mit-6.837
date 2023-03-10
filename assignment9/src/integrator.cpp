#include "integrator.h"
#include "particle.h"
#include "forcefield.h"

EulerIntegrator::EulerIntegrator()
{
    color = {1.f, 0.f, 0.f};
}

void EulerIntegrator::Update(Particle *particle, ForceField *forcefield, float t, float dt)
{
    Vec3f pn = particle->getPosition();
    particle->setPosition(pn + particle->getVelocity() * dt);
    particle->setVelocity(particle->getVelocity() + forcefield->getAcceleration(pn, particle->getMass(), t) * dt);
    particle->increaseAge(dt);
}

MidpointIntegrator::MidpointIntegrator()
{
    color = {0.f, 1.f, 0.f};
}

void MidpointIntegrator::Update(Particle *particle, ForceField *forcefield, float t, float dt)
{
    Vec3f pm = particle->getPosition() + particle->getVelocity() * dt * .5f;
    Vec3f vm = particle->getVelocity() + forcefield->getAcceleration(particle->getPosition(), particle->getMass(), t) * dt * .5f;
    particle->setPosition(particle->getPosition() + vm * dt);
    particle->setVelocity(particle->getVelocity() + forcefield->getAcceleration(pm, particle->getMass(), t + dt * .5f) * dt);
    particle->increaseAge(dt);
}

const Vec3f& Integrator::getColor()
{
    return color;
}

RungeKuttaIntegrator::RungeKuttaIntegrator()
{
    color = {0.f, 0.f, 1.f};
}

void RungeKuttaIntegrator::Update(Particle *particle, ForceField *forcefield, float t, float dt)
{
    Vec3f p[4];
    Vec3f v[4];
    Vec3f a[4];

    p[0] = particle->getPosition();
    v[0] = particle->getVelocity();
    a[0] = forcefield->getAcceleration(p[0], particle->getMass(), t);

    p[1] = p[0] + v[0] * dt * .5f;
    v[1] = v[0] + a[0] * dt * .5f;
    a[1] = forcefield->getAcceleration(p[1], particle->getMass(), t);

    p[2] = p[0] + v[1] * dt * .5f;
    v[2] = v[0] + a[1] * dt * .5f;
    a[2] = forcefield->getAcceleration(p[2], particle->getMass(), t);

    p[3] = p[0] + v[2] * dt;
    v[3] = v[0] + a[2] * dt;
    a[3] = forcefield->getAcceleration(p[3], particle->getMass(), t);

    particle->setPosition(p[0] + dt / 6.f * (v[0] + 2.f * v[1] + 2.f * v[2] + v[3]));
    particle->setVelocity(v[0] + dt / 6.f * (a[0] + 2.f * a[1] + 2.f * a[2] + a[3]));
}
