#include "surface.h"
#include "curve.h"

SurfaceOfRevolution::SurfaceOfRevolution(Curve *c)
{
}

void SurfaceOfRevolution::Paint(ArgParser *args)
{
}

int SurfaceOfRevolution::getNumVertices()
{
    return 0;
}

Vec3f SurfaceOfRevolution::getVertex(int i)
{
    return Vec3f();
}

void SurfaceOfRevolution::setVertex(int i, const Vec3f &v)
{
}

BezierPatch::BezierPatch()
{
}

void BezierPatch::Paint(ArgParser *args)
{
}

void BezierPatch::setVertex(int i, const Vec3f &v)
{
}

int BezierPatch::getNumVertices()
{
    return 0;
}

Vec3f BezierPatch::getVertex(int i)
{
    return Vec3f();
}
