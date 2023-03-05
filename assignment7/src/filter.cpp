#include "filter.h"
#include "film.h"
#include "sample.h"

Vec3f Filter::getColor(int x, int y, Film *film)
{
    int support_radius = getSupportRadius();
    Vec3f color(0.f, 0.f, 0.f);
    float weight_sum = 0.f;

    for (int i = -support_radius; i <= support_radius; ++i)
    {
        if(x + i < 0 || x + i >= film->getWidth())
        {
            continue;
        }

        for (int j = -support_radius; j <= support_radius; ++j)
        {
            if(y + j < 0 || y + j >= film->getHeight())
            {
                continue;
            }

            for (int k = 0; k < film->getNumSamples(); ++k)
            {
                Sample s = film->getSample(x + i, y + j, k);
                float weight = getWeight(i + s.getPosition().x() - .5f, j + s.getPosition().y() - .5f);
                color += weight * s.getColor();
                weight_sum += weight;
            }
        }
    }

    if (weight_sum > 0.f)
    {
        color /= weight_sum;
    }

    return color;
}

BoxFilter::BoxFilter(float r)
    : radius(r)
{
}

float BoxFilter::getWeight(float x, float y)
{
    auto getBoxWeight = [&](float k)
    {
        return k > -radius && k < radius ? 1.f : 0.f;
    };

    return getBoxWeight(x) * getBoxWeight(y);
}

int BoxFilter::getSupportRadius()
{
    return ceilf(radius - .5f);
}

TentFilter::TentFilter(float r)
    : radius(r)
{
}

float TentFilter::getWeight(float x, float y)
{
    auto getTentWeight = [&](float k)
    {
        return clamp(1.f - 1.f / radius * fabs(k), 0.f, 1.f);
    };

    return getTentWeight(sqrtf(x * x + y * y));
}

int TentFilter::getSupportRadius()
{
    return ceilf(radius - .5f);
}

GaussianFilter::GaussianFilter(float s)
    : sigma(s)
{
}

float GaussianFilter::getWeight(float x, float y)
{
    auto getGaussianWeight = [&](float k)
    {
        return clamp(exp(-k * k / (2.f * sigma * sigma)), 0.f, 1.f);
    };

    return getGaussianWeight(sqrtf(x * x + y * y));
}

int GaussianFilter::getSupportRadius()
{
    return ceilf(2 * sigma - .5f);
}
