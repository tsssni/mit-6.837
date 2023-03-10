#ifndef _GENERATOR_H_
#define _GENERATOR_H_

#include "vectors.h"

class Particle;

class Generator
{
public:
    // initialization
    void SetColors(const Vec3f& color, const Vec3f& dead_color, float color_randomness);
    virtual void SetLifespan(float lifespan, float lifespan_randomness, int desired_num_particles);
    void SetMass(float mass, float mass_randomness);

    // on each timestep, create some particles
    virtual int numNewParticles(float current_time, float dt) = 0;
    virtual Particle *Generate(float current_time, int i) = 0;

    // for the gui
    void Paint() const;
    void Restart();

protected:
    Vec3f color = {1.f, 0.f, 0.f};
    Vec3f deadColor = {1.f, 0.f, 0.f};
    float colorRandomness = 0.f;

    float lifespan = 0.f;
    float lifespanRandomness = 0.f;

    float mass = 0.f;
    float massRandomness = 0.f;

    int desiredNumParticles = 0;
};

class HoseGenerator : public Generator
{
public:
    HoseGenerator(const Vec3f &position, float position_randomness, const Vec3f &velocity, float velocity_randomness);
    virtual int numNewParticles(float current_time, float dt) override;
    virtual Particle *Generate(float current_time, int i) override;

protected:
    Vec3f position;
    float positionRandomness = 0.f;

    Vec3f velocity;
    float velocityRandomness = 0.f;
};

class RingGenerator : public Generator
{
public:
    RingGenerator(float position_randomness, const Vec3f& velocity, float velocity_randomness);
    virtual int numNewParticles(float current_time, float dt) override;
    virtual Particle *Generate(float current_time, int i) override;

protected:
    float positionRandomness = 0.f;

    Vec3f velocity;
    float velocityRandomness = 0.f;

    int numParticles;
};
#endif