#include "curve.h"
#include "arg_parser.h"
#include <GL/gl.h>

Matrix BezierCurve::basis = {};
Matrix BSplineCurve::basis = {};

Curve::Curve(int num)
{
    numVertices = num;
    vertices = new Vec3f[num];
}

Curve::~Curve()
{
    if (vertices)
    {
        delete[] vertices;
    }
}

void Curve::setVertex(int i, const Vec3f &v)
{
    vertices[i] = v;
}

void Curve::Paint(ArgParser *args)
{
    glLineWidth(5.f);
    glBegin(GL_LINES);

    glColor3f(1.f * 0x57 / 0xff, 1.f * 0x66 / 0xff, 1.f * 0x90 / 0xff);
    for (int i = 0; i < getNumVertices() - 1; ++i)
    {
        Vec3f vert_begin = getVertex(i);
        Vec3f vert_end = getVertex(i + 1);
        glVertex3f(vert_begin.x(), vert_begin.y(), vert_begin.z());
        glVertex3f(vert_end.x(), vert_end.y(), vert_end.z());
    }

    glColor3f(1.f * 0xb8 / 0xff, 1.f * 0xa6 / 0xff, 1.f * 0xd9 / 0xff);
    int tess = getTessellation(args->curve_tessellation);
    for (int i = 0; i < tess; ++i)
    {
        Vec3f curve_begin = getCurve(1.f * i / tess);
        Vec3f curve_end = getCurve(1.f * (i + 1) / tess);
        glVertex3f(curve_begin.x(), curve_begin.y(), curve_begin.z());
        glVertex3f(curve_end.x(), curve_end.y(), curve_end.z());
    }

    glEnd();

    glPointSize(10.f);
    glBegin(GL_POINTS);

    glColor3f(1.f * 0xe7 / 0xff, 1.f * 0x99 / 0xff, 1.f * 0xb0 / 0xff);
    for (int i = 0; i < getNumVertices(); ++i)
    {
        Vec3f vert = getVertex(i);
        glVertex3f(vert.x(), vert.y(), vert.z());
    }

    glEnd();
}

int Curve::getNumVertices()
{
    return numVertices;
}

Vec3f Curve::getVertex(int i)
{
    assert(i < numVertices);
    return vertices[i];
}

BezierCurve::BezierCurve(int num)
    : Curve(num)
{
}

Vec3f BezierCurve::getCurve(float t)
{
    float curve_t = t * (numVertices - 1);
    int start_control_point_idx = int(curve_t) / 3 * 3;
    float sub_curve_t = (curve_t - start_control_point_idx) / 3;

    Vec4f point(powf(sub_curve_t, 3.f), powf(sub_curve_t, 2.f), sub_curve_t, 1);
    Matrix control_points;

    for (int i = 0; i < 4; ++i)
    {
        const Vec3f &vertex = vertices[start_control_point_idx + i];
        for (int j = 0; j < 3; ++j)
        {
            control_points.Set(i, j, vertex[j]);
        }
    }

    (control_points * basis).Transform(point);
    return {point.x(), point.y(), point.z()};
}

int BezierCurve::getTessellation(int tess)
{
    return tess * (numVertices - 1) / 3;
}

void BezierCurve::initBasis()
{
    basis.Set(0, 0, -1.f);
    basis.Set(1, 0, 3.f);
    basis.Set(2, 0, -3.f);
    basis.Set(3, 0, 1.f);
    basis.Set(0, 1, 3.f);
    basis.Set(1, 1, -6.f);
    basis.Set(2, 1, 3.f);
    basis.Set(3, 1, 0.f);
    basis.Set(0, 2, -3.f);
    basis.Set(1, 2, 3.f);
    basis.Set(2, 2, 0.f);
    basis.Set(3, 2, 0.f);
    basis.Set(0, 3, 1.f);
    basis.Set(1, 3, 0.f);
    basis.Set(2, 3, 0.f);
    basis.Set(3, 3, 0.f);
}

void BezierCurve::OutputBezier(FILE *file)
{
    fprintf(file, "\nbezier");
    fprintf(file, "\nnum_vertices 4\n");

    for (int i = 0; i < numVertices; ++i)
    {
        vertices[i].Write(file);
    };
}

void BezierCurve::OutputBSpline(FILE *file)
{
    fprintf(file, "\nbspline");
    fprintf(file, "\nnum_vertices 4\n");

    auto bspline_basis_inv = BSplineCurve::basis;
    bspline_basis_inv.Inverse();
    Matrix control_points;

    for (int i = 0; i < 4; ++i)
    {
        const Vec3f &vertex = vertices[i];
        for (int j = 0; j < 3; ++j)
        {
            control_points.Set(i, j, vertex[j]);
        }
    }

    control_points *= basis * bspline_basis_inv;

    for (int i = 0; i < 4; ++i)
    {
        Vec3f vertex(control_points.Get(i, 0),
                     control_points.Get(i, 1),
                     control_points.Get(i, 2));

        vertex.Write(file);
    }
}

BSplineCurve::BSplineCurve(int num)
    : Curve(num)
{
}

Vec3f BSplineCurve::getCurve(float t)
{
    float curve_t = t * (numVertices - 3);
    int start_control_point_idx = int(curve_t);
    float sub_curve_t = curve_t - start_control_point_idx;

    Vec4f point(powf(sub_curve_t, 3.f), powf(sub_curve_t, 2.f), sub_curve_t, 1);
    Matrix control_points;

    for (int i = 0; i < 4; ++i)
    {
        const Vec3f &vertex = vertices[start_control_point_idx + i];
        for (int j = 0; j < 3; ++j)
        {
            control_points.Set(i, j, vertex[j]);
        }
    }

    (control_points * basis).Transform(point);
    return {point.x(), point.y(), point.z()};
}

int BSplineCurve::getTessellation(int tess)
{
    return tess * (numVertices - 3);
}

void BSplineCurve::initBasis()
{
    basis.Set(0, 0, -1.f);
    basis.Set(1, 0, 3.f);
    basis.Set(2, 0, -3.f);
    basis.Set(3, 0, 1.f);
    basis.Set(0, 1, 3.f);
    basis.Set(1, 1, -6.f);
    basis.Set(2, 1, 0.f);
    basis.Set(3, 1, 4.f);
    basis.Set(0, 2, -3.f);
    basis.Set(1, 2, 3.f);
    basis.Set(2, 2, 3.f);
    basis.Set(3, 2, 1.f);
    basis.Set(0, 3, 1.f);
    basis.Set(1, 3, 0.f);
    basis.Set(2, 3, 0.f);
    basis.Set(3, 3, 0.f);

    basis *= 1.f / 6.f;
}

void BSplineCurve::OutputBezier(FILE *file)
{
    fprintf(file, "\nbezier");
    fprintf(file, "\nnum_vertices 4\n");

    auto bezier_basis_inv = BezierCurve::basis;
    bezier_basis_inv.Inverse();
    Matrix control_points;

    for (int i = 0; i < 4; ++i)
    {
        const Vec3f &vertex = vertices[i];
        for (int j = 0; j < 3; ++j)
        {
            control_points.Set(i, j, vertex[j]);
        }
    }

    control_points *= basis * bezier_basis_inv;

    for (int i = 0; i < 4; ++i)
    {
        Vec3f vertex(control_points.Get(i, 0),
                     control_points.Get(i, 1),
                     control_points.Get(i, 2));

        vertex.Write(file);
    }
}

void BSplineCurve::OutputBSpline(FILE *file)
{
    fprintf(file, "\nbspline");
    fprintf(file, "\nnum_vertices 4\n");

    for (int i = 0; i < numVertices; ++i)
    {
        vertices[i].Write(file);
    };
}
