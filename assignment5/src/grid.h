#ifndef _GRID_H_
#define _GRID_H_

#include "object3d.h"
#include "vectors.h"
#include "bounding_box.h"
#include "ray.h"
#define CELL_MAT_NUM 32

class Object3DVector;

class MarchingInfo
{
public:
    void nextCell();
    Vec3f getCell();
    const Vec3f &getNormal();

    bool isIntersected();
    float getIntersectedT();
    Material *getIntersectedMat();
    const Vec3f &getIntersectedNormal();

    void setCellPos(int _i, int _j, int _k);
    void setCellPos(const Vec3f &pos);
    void setGrid(Grid *g);
    void setRay(const Ray &r);
    void setNormal(const Vec3f &n);
    void setT(float _t);

protected:
    int i = -1;
    int j = -1;
    int k = -1;

    Grid *grid;
    Ray ray;
    Vec3f normal;
    float t = -1.f;

    bool intersected = false;
    Vec3f intersected_normal;
    float intersected_t;
    Material *intersected_mat;
};

class Grid : public Object3D
{
public:
    Grid(BoundingBox *bb, int _nx, int _ny, int _nz);
    ~Grid();

    virtual bool intersect(const Ray &r, Hit &h, float tmin) override;
    virtual void paint() override;

    int getX();
    int getY();
    int getZ();

    Object3DVector *getCellObjects(int x, int y, int z);
    Object3DVector *getCellObjects(const Vec3f &pos);
    Vec3f getCellInfo();
    Vec3f getCellCenter(int x, int y, int z);
    Vec3f getCellCenter(const Vec3f &pos);
    Vec3f getCellPos(float x, float y, float z);
    Vec3f getCellPos(const Vec3f &pos);
    BoundingBox getCellBox(int x, int y, int z);
    BoundingBox getCellBox(const Vec3f &pos);
    Material *getCellMat(int x, int y, int z);
    Material *getCellMat(const Vec3f &pos);

    void setCell(int x, int y, int z, Object3D *obj);
    void setCell(const Vec3f &pos, Object3D *obj);

protected:
    void initializeRayMarch(MarchingInfo &mi, const Ray &r, float tmin);
    bool testCellInGrid(int x, int y, int z);
    bool testCellInGrid(const Vec3f &pos);
    int getCellIdx(int x, int y, int z);
    int getCellIdx(const Vec3f &pos);

    void glDrawFace(const Vec3f &pos, const Vec3f &normal);
    void rayTreeDrawCell(const Vec3f &pos);
    void rayTreeDrawFace(const Vec3f &pos, const Vec3f &normal);

    Object3DVector **vector;
    Material *cell_mat[CELL_MAT_NUM];

    int nx;
    int ny;
    int nz;

    float nl;
    float nh;
    float nw;
};
#endif