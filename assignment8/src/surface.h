#ifndef _SURFACE_H_
#define _SURFACE_H_

#include "spline.h"

class Curve;

class Surface : public Spline
{
};

class SurfaceOfRevolution : public Surface
{
public:
    SurfaceOfRevolution(Curve* c);
    virtual void Paint(ArgParser *args) override;
    virtual int getNumVertices() override;
    virtual Vec3f getVertex(int i) override;
    virtual void setVertex(int i, const Vec3f &v) override;
};

class BezierPatch : public Surface
{
public:
    BezierPatch();
    virtual void Paint(ArgParser *args) override;
    virtual int getNumVertices() override;
    virtual Vec3f getVertex(int i) override;
    virtual void setVertex(int i, const Vec3f &v) override;
};
#endif