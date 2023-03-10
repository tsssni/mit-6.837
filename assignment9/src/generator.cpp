#include "generator.h"
#include "particle.h"
#include "random.h"
#include <GL/gl.h>

Random randomGenerator;

void Generator::SetColors(const Vec3f& _color, const Vec3f& dead_color, float color_randomness)
{
    color = _color;
    deadColor = dead_color;
    colorRandomness = color_randomness;
}

void Generator::SetLifespan(float _lifespan, float lifespan_randomness, int desired_num_particles)
{
    lifespan = _lifespan;
    lifespanRandomness = lifespan_randomness;

    desiredNumParticles = desired_num_particles;
}

void Generator::SetMass(float _mass, float mass_randomness)
{
    mass = _mass;
    massRandomness = mass_randomness;
}

void Generator::Paint() const
{
}

void Generator::Restart()
{
}

HoseGenerator::HoseGenerator(const Vec3f& _position, float position_randomness, const Vec3f& _velocity, float velocity_randomness)
{
    position = _position;
    positionRandomness = position_randomness;

    velocity = _velocity;
    velocityRandomness = velocity_randomness;
}

RingGenerator::RingGenerator(float position_randomness, const Vec3f& _velocity, float velocity_randomness)
{
    positionRandomness = position_randomness;

    velocity = _velocity;
    velocityRandomness = velocity_randomness;
}

int RingGenerator::numNewParticles(float current_time, float dt)
{
    numParticles = roundf(desiredNumParticles * current_time / lifespan * dt);
    return numParticles;
}

Particle *RingGenerator::Generate(float current_time, int i)
{
    Vec3f generate_position = Vec3f(current_time * cosf(2.f * M_PI *  i / numParticles),
                                    current_time * sinf(2.f * M_PI *  i / numParticles),
                                    0.f) +
                              positionRandomness * randomGenerator.randomVector();
    Vec3f generate_velocity = velocity + velocityRandomness * randomGenerator.randomVector();
    Vec3f generate_color = color + colorRandomness * randomGenerator.randomVector();
    Vec3f generate_dead_color = deadColor + colorRandomness * randomGenerator.randomVector();
    float generate_mass = mass + massRandomness * randomGenerator.next();
    float generate_lifespan = lifespan + lifespanRandomness * randomGenerator.next();

    return new Particle(generate_position,
                        generate_velocity,
                        generate_color,
                        generate_dead_color,
                        generate_mass,
                        generate_lifespan);
}

int HoseGenerator::numNewParticles(float current_time, float dt)
{
    return roundf(desiredNumParticles / lifespan * dt);
}

Particle *HoseGenerator::Generate(float current_time, int i)
{
    Vec3f generate_position = position + positionRandomness * randomGenerator.randomVector();
    Vec3f generate_velocity = velocity + velocityRandomness * randomGenerator.randomVector();
    Vec3f generate_color = color + colorRandomness * randomGenerator.randomVector();
    Vec3f generate_dead_color = deadColor + colorRandomness * randomGenerator.randomVector();
    float generate_mass = mass + massRandomness * randomGenerator.next();
    float generate_lifespan = lifespan + lifespanRandomness * randomGenerator.next();

    return new Particle(generate_position,
                          generate_velocity,
                          generate_color,
                          generate_dead_color,
                          generate_mass,
                          generate_lifespan);
}
