#include "spline.h"
#include "arg_parser.h"
#include "triangle_mesh.h"

void Spline::OutputBezier(FILE *file)
{
}

void Spline::OutputBSpline(FILE *file)
{
}

void Spline::moveControlPoint(int selectedPoint, float x, float y)
{
    setVertex(selectedPoint, {x, y, 0.f});
}

void Spline::addControlPoint(int selectedPoint, float x, float y)
{
    assert(vertices);

    Vec3f *old_points = vertices;
    Vec3f *new_points = new Vec3f[numVertices + 1];

    for (int i = selectedPoint + 1; i < numVertices + 1; ++i)
    {
        new_points[i] = old_points[i - 1];
    }

    for (int i = 0; i < selectedPoint; ++i)
    {
        new_points[i] = old_points[i];
    }

    new_points[selectedPoint] = {x, y, 0.f};
    numVertices += 1;
    vertices = new_points;
    delete[] old_points;
}

void Spline::deleteControlPoint(int selectedPoint)
{
    assert(vertices);
    Vec3f *old_points = vertices;
    vertices = NULL;

    if (numVertices > 1)
    {
        Vec3f *new_points = new Vec3f[numVertices - 1];

        for (int i = selectedPoint; i < numVertices - 1; ++i)
        {
            new_points[i] = old_points[i + 1];
        }

        for (int i = 0; i < selectedPoint; ++i)
        {
            new_points[i] = old_points[i];
        }

        vertices = new_points;
    }

    numVertices -= 1;
    delete[] old_points;
}

TriangleMesh *Spline::OutputTriangles(ArgParser *args)
{
    return nullptr;
}
