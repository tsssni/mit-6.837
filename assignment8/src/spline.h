#ifndef _SPLINE_H_
#define _SPLINE_H_

#include "vectors.h"
#include <stdio.h>

class ArgParser;
class TriangleMesh;

class Spline
{
public:
    // FOR VISUALIZATION
    virtual void Paint(ArgParser *args) = 0;

    // FOR CONVERTING BETWEEN SPLINE TYPES
    virtual void OutputBezier(FILE *file);
    virtual void OutputBSpline(FILE *file);

    // FOR CONTROL POINT PICKING
    virtual int getNumVertices() = 0;
    virtual Vec3f getVertex(int i) = 0;
    virtual void setVertex(int i, const Vec3f &v) = 0;

    // FOR EDITING OPERATIONS
    virtual void moveControlPoint(int selectedPoint, float x, float y);
    virtual void addControlPoint(int selectedPoint, float x, float y);
    virtual void deleteControlPoint(int selectedPoint);

    // FOR GENERATING TRIANGLES
    virtual TriangleMesh *OutputTriangles(ArgParser *args);

protected:
    int numVertices;
    Vec3f *vertices;
};
#endif