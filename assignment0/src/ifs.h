#include "utils/vectors.h"
#include "utils/matrix.h"
#include "utils/image.h"

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