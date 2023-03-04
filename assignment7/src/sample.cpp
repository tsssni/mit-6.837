#include "sample.h"
#include <math.h>

Sampler::Sampler(int n)
    : num(n)
{
}

RandomSampler::RandomSampler(int n)
    : Sampler(n)
{
}

Vec2f RandomSampler::getSamplePosition(int n)
{
    return Vec2f(rand() * 1.f / RAND_MAX, rand() * 1.f / RAND_MAX);
}

UniformSampler::UniformSampler(int n)
    : Sampler(n)
{
    samples = new Vec2f[n];
    int size = sqrt(n);
    float offset = .5f / size;

    for (int i = 0; i < n; ++i)
    {
        float x = 1.f * (i / size) / size + offset;
        float y = 1.f * (i % size) / size + offset;
        samples[i] = {x, y};
    }
}

UniformSampler::~UniformSampler()
{
    if (samples)
    {
        delete[] samples;
    }
}

Vec2f UniformSampler::getSamplePosition(int n)
{
    return samples[n];
}

JitteredSampler::JitteredSampler(int n)
    : Sampler(n)
{
    samples = new Vec2f[n];
    size = sqrt(n);
    float offset = .5f / size;

    for (int i = 0; i < n; ++i)
    {
        float x = 1.f * (i / size) / size + offset;
        float y = 1.f * (i % size) / size + offset;
        samples[i] = {x, y};
    }
}

JitteredSampler::~JitteredSampler()
{
    if(samples)
    {
        delete samples;
    }
}

Vec2f JitteredSampler::getSamplePosition(int n)
{
    float x = samples[n].x() + 1.f / size * (rand() * 1.f / RAND_MAX - .5f);
    float y = samples[n].y() + 1.f / size * (rand() * 1.f / RAND_MAX - .5f);

    return {x, y};
}
