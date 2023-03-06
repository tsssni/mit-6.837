#include "surface.h"
#include "curve.h"
#include "triangle_mesh.h"
#include "arg_parser.h"
#include "matrix.h"

SurfaceOfRevolution::SurfaceOfRevolution(Curve *c)
    : curve(c)
{
}

void SurfaceOfRevolution::Paint(ArgParser *args)
{
    curve->Paint(args);
}

int SurfaceOfRevolution::getNumVertices()
{
    return curve->getNumVertices();
}

Vec3f SurfaceOfRevolution::getVertex(int i)
{
    return curve->getVertex(i);
}

void SurfaceOfRevolution::setVertex(int i, const Vec3f &v)
{
    curve->setVertex(i, v);
}

TriangleMesh *SurfaceOfRevolution::OutputTriangles(ArgParser *args)
{
    int tess = curve->getTessellation(args->curve_tessellation);
    net = new TriangleNet(args->revolution_tessellation, tess);

    for (int i = 0; i <= args->revolution_tessellation; ++i)
    {
        Matrix rotation = Matrix::MakeYRotation(2.f * M_PI * i / args->revolution_tessellation);

        for (int j = 0; j <= tess; ++j)
        {
            Vec3f curve_point = curve->getCurve(1.f * j / tess);
            rotation.Transform(curve_point);

            net->SetVertex(i, j, curve_point);
        }
    }

    return net;
}

void SurfaceOfRevolution::OutputBezier(FILE *file)
{
    fprintf(file, "\nsurface_of_revolution");
    curve->OutputBezier(file);
}

void SurfaceOfRevolution::OutputBSpline(FILE *file)
{
    fprintf(file, "\nsurface_of_revolution");
    curve->OutputBSpline(file);
}

void SurfaceOfRevolution::moveControlPoint(int selectedPoint, float x, float y)
{
    curve->moveControlPoint(selectedPoint, x, y);
}

void SurfaceOfRevolution::addControlPoint(int selectedPoint, float x, float y)
{
    curve->addControlPoint(selectedPoint, x, y);
}

void SurfaceOfRevolution::deleteControlPoint(int selectedPoint)
{
    curve->deleteControlPoint(selectedPoint);
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

TriangleMesh *BezierPatch::OutputTriangles(ArgParser *args)
{
    return nullptr;
}

void BezierPatch::moveControlPoint(int selectedPoint, float x, float y)
{
}

void BezierPatch::addControlPoint(int selectedPoint, float x, float y)
{
}

void BezierPatch::deleteControlPoint(int selectedPoint)
{
}

void BezierPatch::OutputBezier(FILE *file)
{
}
void BezierPatch::OutputBSpline(FILE *file)
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

Surface::~Surface()
{
    if (net)
    {
        delete net;
    }
}
