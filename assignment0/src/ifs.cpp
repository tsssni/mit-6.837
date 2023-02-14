#include "ifs.h"
#include "vectors.h"
#include "matrix.h"
#include "image.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

IFS::IFS()
{
}

IFS::~IFS()
{
    delete[] mat;
    delete[] p;

    mat = nullptr;
    p = nullptr;
}

void IFS::Read(FILE *F)
{
    assert(F != NULL);

    int num;
    fscanf(F, "%d", &num);

    mat = new Matrix[num];
    p = new float[num];

    for (int i = 0; i < num; ++i)
    {
        fscanf(F, "%f", p + i);
        mat[i].Read3x3(F);
    }
}

void IFS::Render(Image &img, int num_points, int num_iters)
{
    Vec3f color(.5f, .5f, .5f);

    for (int i = 0; i < num_points; ++i)
    {
        float x = rand() * 1.f / RAND_MAX;
        float y = rand() * 1.f / RAND_MAX;
        Vec2f pt(x, y);

        for (int j = 0; j < num_iters; ++j)
        {
            GetRandomTransformation().Transform(pt);
        }

        if (pt.x() >= 0.f && pt.x() < 1.f && pt.y() >= 0.f && pt.y() < 1.f)
        {
            img.SetPixel(
                pt.x() * img.Width(),
                pt.y() * img.Height(),
                color);
        }
    }
}

Matrix &IFS::GetRandomTransformation()
{
    float prob = rand() * 1.f / RAND_MAX;
    float interval_left = 0.f;
    float interval_right = 0.f;
    int i = 0;

    while (interval_right < 1.f)
    {
        interval_right += p[i];

        if (prob >= interval_left && prob < interval_right)
        {
            break;
        }

        interval_left = interval_right;
        ++i;
    }

    return mat[i];
}
