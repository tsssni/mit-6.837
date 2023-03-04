#include "filter.h"
#include "film.h"
#include "sample.h"

Vec3f Filter::getColor(int x, int y, Film *film)
{
    int support_radius = getSupportRadius();
    Vec3f color(0.f, 0.f, 0.f);
    float weight_sum = 0.f;

    for (int i = -support_radius; i <= support_radius && x + i >= 0 && x + i < film->getWidth(); ++i)
    {
        for (int j = -support_radius; j <= support_radius && y + j >= 0 && y + j < film->getHeight(); ++j)
        {
            for (int k = 0; k < film->getNumSamples(); ++k)
            {
                Sample s = film->getSample(x + i, y + j, k);
                float weight = getWeight(i + s.getPosition().x(), j + s.getPosition().y());
                color += weight * s.getColor();
                weight_sum += weight;
            }
        }
    }

    color /= weight_sum;
    return color;
}

BoxFilter::BoxFilter(float r)
    : radius(r)
{
}

float BoxFilter::getWeight(float x, float y)
{
    auto getBoxWeight = [&](float x)
    {
        return x > -radius && x < radius ? 1.f / radius : 0.f;
    };

    return getBoxWeight(x) * getBoxWeight(y);
}

int BoxFilter::getSupportRadius()
{
    return radius;
}

TentFilter::TentFilter(float r)
    : radius(r)
{
}

float TentFilter::getWeight(float x, float y)
{
    auto getTentWeight = [&](float x)
    {
        return clamp(1.f - 1.f / radius * fabs(x), 0.f, 1.f);
    };

    return getTentWeight(x) * getTentWeight(y);
}

int TentFilter::getSupportRadius()
{
    return radius;
}

GaussianFilter::GaussianFilter(float s)
    : sigma(s)
{
}

float GaussianFilter::getWeight(float x, float y)
{
    auto getGaussianWeight = [&](float x)
    {
        return clamp(exp(-x * x * .5f * sigma), 0.f, 1.f);
    };

    return getGaussianWeight(x) * getGaussianWeight(y);
}

int GaussianFilter::getSupportRadius()
{
    return 2 * sigma;
}
