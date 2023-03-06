#ifndef _FILTER_H_
#define _FILTER_H_

#include "vectors.h"

class Film;

class Filter
{
public:
    Vec3f getColor(int i, int j, Film *film);
    virtual float getWeight(float x, float y) = 0;
    virtual int getSupportRadius() = 0;
};

class BoxFilter : public Filter
{
public:
    BoxFilter(float r);
    virtual float getWeight(float x, float y) override;
    virtual int getSupportRadius() override;

protected:
    float radius;
};

class TentFilter : public Filter
{
public:
    TentFilter(float r);
    virtual float getWeight(float x, float y) override;
    virtual int getSupportRadius() override;

protected:
    float radius;
};

class GaussianFilter : public Filter
{
public:
    GaussianFilter(float s);
    virtual float getWeight(float x, float y) override;
    virtual int getSupportRadius() override;

protected:
    float sigma;
};
#endif