#ifndef _SURFACE_H_
#define _SURFACE_H_

#include "spline.h"

class Curve;
class TriangleNet;

class Surface : public Spline
{
public:
    ~Surface();

protected:
    TriangleNet *net;
};

class SurfaceOfRevolution : public Surface
{
public:
    SurfaceOfRevolution(Curve* c);
    virtual void Paint(ArgParser *args) override;

    virtual void OutputBezier(FILE *file);
    virtual void OutputBSpline(FILE *file);

    virtual int getNumVertices() override;
    virtual Vec3f getVertex(int i) override;
    virtual void setVertex(int i, const Vec3f &v) override;

    virtual void moveControlPoint(int selectedPoint, float x, float y) override;
    virtual void addControlPoint(int selectedPoint, float x, float y) override;
    virtual void deleteControlPoint(int selectedPoint) override;
    
    virtual TriangleMesh *OutputTriangles(ArgParser *args);

protected:
    Curve *curve;
};

class BezierPatch : public Surface
{
public:
    BezierPatch();
    virtual void Paint(ArgParser *args) override;

    virtual void OutputBezier(FILE *file);
    virtual void OutputBSpline(FILE *file);

    virtual int getNumVertices() override;
    virtual Vec3f getVertex(int i) override;
    virtual void setVertex(int i, const Vec3f &v) override;

    virtual void moveControlPoint(int selectedPoint, float x, float y) override;
    virtual void addControlPoint(int selectedPoint, float x, float y) override;
    virtual void deleteControlPoint(int selectedPoint) override;
    
    virtual TriangleMesh *OutputTriangles(ArgParser *args);
};
#endif