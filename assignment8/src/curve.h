#ifndef _CURVE_H_
#define _CURVE_H_

#include "spline.h"
#include "matrix.h"

class Curve : public Spline
{
public:
    Curve(int num);
    ~Curve();

    virtual void setVertex(int i, const Vec3f &v) override;
    virtual void Paint(ArgParser *args) override;

    virtual int getNumVertices() override;
    virtual Vec3f getVertex(int i) override;

protected:
    virtual Vec3f getCurve(float t) = 0;
    virtual int getTessellation(int tess) = 0;
};

class BSplineCurve;

class BezierCurve : public Curve
{
    friend BSplineCurve;

public:
    BezierCurve(int num);
    static void initBasis();

    virtual void OutputBezier(FILE *file) override;
    virtual void OutputBSpline(FILE *file) override;

protected:
    virtual Vec3f getCurve(float t) override;
    virtual int getTessellation(int tess) override;

    static Matrix basis;
};

class BSplineCurve : public Curve
{
    friend BezierCurve;

public:
    BSplineCurve(int num);
    static void initBasis();

    virtual void OutputBezier(FILE *file) override;
    virtual void OutputBSpline(FILE *file) override;

protected:
    virtual Vec3f getCurve(float t) override;
    virtual int getTessellation(int tess) override;

    static Matrix basis;
};
#endif