#ifndef _IFS_H_
#define _IFS_H_

#include <stdio.h>

class Image;
class Matrix;

class IFS
{
public:
    IFS();
    ~IFS();
    void Read(FILE *F);
    void Render(Image &img, int num_points, int num_iters);

private:
    Matrix &GetRandomTransformation();

    Matrix* mat = nullptr;
    float* p = nullptr;
};

#endif
