#include "grid.h"
#include "bounding_box.h"
#include "ray.h"
#include "hit.h"
#include "ray_tree.h"
#include "object3d_vector.h"
#include <GL/gl.h>
#include <float.h>
#include <algorithm>
#define EPLISON 1e-6

Vec3f getTMin(const BoundingBox &bb, const Ray &ray)
{
    Vec3f box_min = bb.getMin();
    Vec3f box_max = bb.getMax();

    const Vec3f &o = ray.getOrigin();
    const Vec3f &d = ray.getDirection();

    float x = d.x() >= 0.f ? box_min.x() : box_max.x();
    float y = d.y() >= 0.f ? box_min.y() : box_max.y();
    float z = d.z() >= 0.f ? box_min.z() : box_max.z();

    Vec3f box_min_t = Vec3f(x, y, z) - o;
    box_min_t.Divide(d.x(), d.y(), d.z());

    return box_min_t;
}

Vec3f getTMax(const BoundingBox &bb, const Ray &ray)
{
    Vec3f box_min = bb.getMin();
    Vec3f box_max = bb.getMax();

    const Vec3f &o = ray.getOrigin();
    const Vec3f &d = ray.getDirection();

    float x = d.x() >= 0.f ? box_max.x() : box_min.x();
    float y = d.y() >= 0.f ? box_max.y() : box_min.y();
    float z = d.z() >= 0.f ? box_max.z() : box_min.z();

    Vec3f box_max_t = Vec3f(x, y, z) - o;
    box_max_t.Divide(d.x(), d.y(), d.z());

    return box_max_t;
}

Vec3f getTNormal(float t, Vec3f t_axes, Vec3f dir)
{
    if (t == t_axes.x())
    {
        int sign = dir.x() >= 0.f ? 1 : -1;
        return -sign * Vec3f(1.f, 0.f, 0.f);
    }
    else if (t == t_axes.y())
    {
        int sign = dir.y() >= 0.f ? 1 : -1;
        return -sign * Vec3f(0.f, 1.f, 0.f);
    }
    else
    {
        int sign = dir.z() >= 0.f ? 1 : -1;
        return -sign * Vec3f(0.f, 0.f, 1.f);
    }
}

Grid::Grid(BoundingBox *bb, int _nx, int _ny, int _nz)
    : nx(_nx), ny(_ny), nz(_nz)
{
    boundingBox = bb;
    vector = new Object3DVector *[nx * ny * nz]
    { NULL };

    Vec3f box_min = bb->getMin();
    Vec3f box_max = bb->getMax();

    nl = (box_max.x() - box_min.x()) / nx;
    nh = (box_max.y() - box_min.y()) / ny;
    nw = (box_max.z() - box_min.z()) / nz;

    Vec3f col[7] = {
        {1.f, 1.f, 1.f},
        {0.f, 0.f, 1.f},
        {0.f, 1.f, 1.f},
        {0.f, 1.f, 0.f},
        {1.f, 1.f, 0.f},
        {1.f, .5f, 0.f},
        {1.f, 0.f, 0.f}};
    Vec3f black(0.f, 0.f, 0.f);

    for (int i = 0; i < CELL_MAT_NUM; ++i)
    {
        int col_i = floor(6.f * i / CELL_MAT_NUM);
        int col_j = ceil(6.f * i / CELL_MAT_NUM);

        int mat_i = CELL_MAT_NUM * col_i / 6.f;
        int mat_j = CELL_MAT_NUM * col_j / 6.f;

        float blend_factor = 1.f * (i - mat_i) / (mat_j - mat_i == 0.f ? 1.f : mat_j - mat_i);
        Vec3f blend_col = (1.f - blend_factor) * col[col_i] + blend_factor * col[col_j];
        cell_mat[i] = new PhongMaterial(blend_col, black, 1.f, black, black, 1.f);
    }
}

Grid::~Grid()
{
    if (vector)
    {
        delete[] vector;
    }
}

void Grid::rayTreeDrawFace(const Vec3f &pos, const Vec3f &normal)
{
    float l = nl * .5f;
    float h = nh * .5f;
    float w = nw * .5f;
    Vec3f center = getCellCenter(pos);
    Material *mat = getCellObjects(pos) ? getCellMat(pos) : cell_mat[0];

    if (normal.x() == -1.f)
    {
        RayTree::AddEnteredFace(
            center + Vec3f(-l, h, -w),
            center + Vec3f(-l, h, w),
            center + Vec3f(-l, -h, w),
            center + Vec3f(-l, -h, -w),
            {-1.f, 0.f, 0.f},
            mat);
    }
    else if (normal.x() == 1.f)
    {
        RayTree::AddEnteredFace(
            center + Vec3f(l, h, w),
            center + Vec3f(l, h, -w),
            center + Vec3f(l, -h, -w),
            center + Vec3f(l, -h, w),
            {1.f, 0.f, 0.f},
            mat);
    }
    else if (normal.y() == -1.f)
    {
        RayTree::AddEnteredFace(
            center + Vec3f(-l, -h, -w),
            center + Vec3f(l, -h, -w),
            center + Vec3f(l, -h, w),
            center + Vec3f(-l, -h, w),
            {0.f, -1.f, 0.f},
            mat);
    }
    else if (normal.y() == 1.f)
    {
        RayTree::AddEnteredFace(
            center + Vec3f(-l, h, w),
            center + Vec3f{l, h, w},
            center + Vec3f(l, h, -w),
            center + Vec3f(-l, h, -w),
            {0.f, 1.f, 0.f},
            mat);
    }
    else if (normal.z() == -1.f)
    {
        RayTree::AddEnteredFace(
            center + Vec3f(-l, h, -w),
            center + Vec3f(l, h, -w),
            center + Vec3f(l, -h, -w),
            center + Vec3f(-l, -h, -w),
            {0.f, 0.f, -1.f},
            mat);
    }
    else
    {
        RayTree::AddEnteredFace(
            center + Vec3f(l, h, w),
            center + Vec3f(-l, h, w),
            center + Vec3f(-l, -h, w),
            center + Vec3f(l, -h, w),
            {0.f, 0.f, 1.f},
            mat);
    }
}

bool Grid::intersect(const Ray &r, Hit &h, float tmin)
{
    MarchingInfo info;
    initializeRayMarch(info, r, tmin);

    while (!info.isIntersected() && testCellInGrid(info.getCell()))
    {
        rayTreeDrawCell(info.getCell());
        rayTreeDrawFace(info.getCell(), info.getNormal());
        info.nextCell();
    }

    if (info.isIntersected())
    {
        h.set(info.getIntersectedT(), info.getIntersectedMat(), info.getIntersectedNormal(), r);
    }

    return info.isIntersected();
}

void Grid::paint()
{
    for (int i = 0; i < nx; ++i)
    {
        for (int j = 0; j < ny; ++j)
        {
            for (int k = 0; k < nz; ++k)
            {
                Vec3f pos = {float(i), float(j), float(k)};

                if (getCellObjects(i, j, k))
                {
                    Material *mat = getCellMat(i, j, k);
                    mat->glSetMaterial();

                    if (!getCellObjects(i - 1, j, k))
                    {
                        glDrawFace(pos, {-1.f, 0.f, 0.f});
                    }

                    if (!getCellObjects(i + 1, j, k))
                    {
                        glDrawFace(pos, {1.f, 0.f, 0.f});
                    }

                    if (!getCellObjects(i, j - 1, k))
                    {
                        glDrawFace(pos, {0.f, -1.f, 0.f});
                    }

                    if (!getCellObjects(i, j + 1, k))
                    {
                        glDrawFace(pos, {0.f, 1.f, 0.f});
                    }

                    if (!getCellObjects(i, j, k - 1))
                    {
                        glDrawFace(pos, {0.f, 0.f, -1.f});
                    }

                    if (!getCellObjects(i, j, k + 1))
                    {
                        glDrawFace(pos, {0.f, 0.f, 1.f});
                    }
                }
            }
        }
    }
}

void Grid::glDrawFace(const Vec3f &pos, const Vec3f &normal)
{
    float l = nl * .5f;
    float h = nh * .5f;
    float w = nw * .5f;
    Vec3f center = getCellCenter(pos);

    glBegin(GL_QUADS);

    if (normal.x() == -1.f)
    {
        glFlatShade(
            {-1.f, 0.f, 0.f},
            center + Vec3f(-l, h, w),
            center + Vec3f(-l, h, -w),
            center + Vec3f(-l, -h, -w),
            center + Vec3f(-l, -h, w));
    }
    else if (normal.x() == 1.f)
    {
        glFlatShade(
            {1.f, 0.f, 0.f},
            center + Vec3f(l, h, -w),
            center + Vec3f(l, h, w),
            center + Vec3f(l, -h, w),
            center + Vec3f(l, -h, -w));
    }
    else if (normal.y() == -1.f)
    {
        glFlatShade(
            {0.f, -1.f, 0.f},
            center + Vec3f(-l, -h, -w),
            center + Vec3f(l, -h, -w),
            center + Vec3f(l, -h, w),
            center + Vec3f(-l, -h, w));
    }
    else if (normal.y() == 1.f)
    {
        glFlatShade(
            {0.f, 1.f, 0.f},
            center + Vec3f(-l, h, w),
            center + Vec3f{l, h, w},
            center + Vec3f(l, h, -w),
            center + Vec3f(-l, h, -w));
    }
    else if (normal.z() == -1.f)
    {
        glFlatShade(
            {0.f, 0.f, -1.f},
            center + Vec3f(-l, h, -w),
            center + Vec3f(l, h, -w),
            center + Vec3f(l, -h, -w),
            center + Vec3f(-l, -h, -w));
    }
    else if (normal.z() == 1.f)
    {
        glFlatShade(
            {0.f, 0.f, 1.f},
            center + Vec3f(l, h, w),
            center + Vec3f(-l, h, w),
            center + Vec3f(-l, -h, w),
            center + Vec3f(l, -h, w));
    }

    glEnd();
}

void Grid::initializeRayMarch(MarchingInfo &mi, const Ray &r, float tmin)
{
    Vec3f tmin_axes = getTMin(*boundingBox, r);
    Vec3f tmax_axes = getTMax(*boundingBox, r);

    float r_tmin = fmax(tmin_axes.x(), fmax(tmin_axes.y(), tmin_axes.z()));
    float r_tmax = fmin(tmax_axes.x(), fmin(tmax_axes.y(), tmax_axes.z()));

    if (r_tmax > 0.f)
    {
        Vec3f p = getCellPos(r_tmin > 0.f ? r.pointAtParameter(r_tmin) : r.getOrigin());
        Vec3f normal = getTNormal(r_tmin, tmin_axes, r.getDirection());

        if (p.x() == nx && normal.x() == 1.f)
        {
            p -= {1.f, 0.f, 0.f};
        }

        if (p.x() == -1 && normal.x() == -1.f)
        {
            p += {1.f, 0.f, 0.f};
        }

        if (p.y() == ny && normal.y() == 1.f)
        {
            p -= {0.f, 1.f, 0.f};
        }

        if (p.y() == -1 && normal.y() == -1.f)
        {
            p += {0.f, 1.f, 0.f};
        }

        if (p.z() == nz && normal.z() == 1.f)
        {
            p -= {0.f, 0.f, 1.f};
        }

        if (p.z() == -1 && normal.z() == -1.f)
        {
            p += {0.f, 0.f, 1.f};
        }

        mi.setCellPos(p);
        mi.setGrid(this);
        mi.setRay(r);
        mi.setNormal(normal);
        mi.setT(fmax(r_tmin, 0.f));
    }
}

void Grid::rayTreeDrawCell(const Vec3f &pos)
{
    float l = nl * .5f;
    float h = nh * .5f;
    float w = nw * .5f;
    Vec3f center = getCellCenter(pos);
    Material *mat = getCellObjects(pos) ? getCellMat(pos) : cell_mat[0];

    RayTree::AddHitCellFace(
        center + Vec3f(-l, h, -w),
        center + Vec3f(-l, h, w),
        center + Vec3f(-l, -h, w),
        center + Vec3f(-l, -h, -w),
        {-1.f, 0.f, 0.f},
        mat);

    RayTree::AddHitCellFace(
        center + Vec3f(l, h, w),
        center + Vec3f(l, h, -w),
        center + Vec3f(l, -h, -w),
        center + Vec3f(l, -h, w),
        {1.f, 0.f, 0.f},
        mat);

    RayTree::AddHitCellFace(
        center + Vec3f(-l, h, -w),
        center + Vec3f(-l, h, w),
        center + Vec3f(-l, -h, w),
        center + Vec3f(-l, -h, -w),
        {-1.f, 0.f, 0.f},
        mat);

    RayTree::AddHitCellFace(
        center + Vec3f(-l, h, w),
        center + Vec3f{l, h, w},
        center + Vec3f(l, h, -w),
        center + Vec3f(-l, h, -w),
        {0.f, 1.f, 0.f},
        mat);

    RayTree::AddHitCellFace(
        center + Vec3f(-l, h, -w),
        center + Vec3f(l, h, -w),
        center + Vec3f(l, -h, -w),
        center + Vec3f(-l, -h, -w),
        {0.f, 0.f, -1.f},
        mat);

    RayTree::AddHitCellFace(
        center + Vec3f(l, h, w),
        center + Vec3f(-l, h, w),
        center + Vec3f(-l, -h, w),
        center + Vec3f(l, -h, w),
        {0.f, 0.f, 1.f},
        mat);
}

int Grid::getX()
{
    return nx;
}

int Grid::getY()
{
    return ny;
}

int Grid::getZ()
{
    return nz;
}

Vec3f Grid::getCellInfo()
{
    Vec3f box_min = boundingBox->getMin();
    Vec3f box_max = boundingBox->getMax();
    Vec3f cell_info = box_max - box_min;
    cell_info.Divide(nx, ny, nz);
    return cell_info;
}

Vec3f Grid::getCellCenter(int x, int y, int z)
{
    Vec3f box_min = boundingBox->getMin();
    return Vec3f(box_min.x() + (x + .5f) * nl,
                 box_min.y() + (y + .5f) * nh,
                 box_min.z() + (z + .5f) * nw);
}

Vec3f Grid::getCellCenter(const Vec3f &pos)
{
    return getCellCenter(pos.x(), pos.y(), pos.z());
}

Vec3f Grid::getCellPos(float x, float y, float z)
{
    Vec3f box_min = boundingBox->getMin();
    return Vec3f(floorf((x - box_min.x()) / nl),
                 floorf((y - box_min.y()) / nh),
                 floorf((z - box_min.z()) / nw));
}

Vec3f Grid::getCellPos(const Vec3f &pos)
{
    return getCellPos(pos.x(), pos.y(), pos.z());
}

BoundingBox Grid::getCellBox(int x, int y, int z)
{
    Vec3f box_min = boundingBox->getMin();
    Vec3f cell_box_min = Vec3f(box_min.x() + x * nl, box_min.y() + y * nh, box_min.z() + z * nw);
    Vec3f cell_box_max = cell_box_min + Vec3f(nl, nh, nw);
    return BoundingBox(cell_box_min, cell_box_max);
}

BoundingBox Grid::getCellBox(const Vec3f &pos)
{
    return getCellBox(pos.x(), pos.y(), pos.z());
}

Material *Grid::getCellMat(int x, int y, int z)
{
    return cell_mat[clamp(getCellObjects(x, y, z)->getNumObjects() - 1, 0, CELL_MAT_NUM - 1)];
}

Material *Grid::getCellMat(const Vec3f &pos)
{
    return getCellMat(pos.x(), pos.y(), pos.z());
}

void Grid::setCell(int x, int y, int z, Object3D *obj)
{
    assert(testCellInGrid(x, y, z));
    int idx = x * ny * nz + y * nz + z;

    if (!vector[idx])
    {
        vector[idx] = new Object3DVector();
    }
    vector[idx]->addObject(obj);
}

void Grid::setCell(const Vec3f &pos, Object3D *obj)
{
    setCell(pos.x(), pos.y(), pos.z(), obj);
}

Object3DVector *Grid::getCellObjects(int x, int y, int z)
{
    if (testCellInGrid(x, y, z))
    {
        return vector[getCellIdx(x, y, z)];
    }
    else
    {
        return nullptr;
    }
}

Object3DVector *Grid::getCellObjects(const Vec3f &pos)
{
    return getCellObjects(pos.x(), pos.y(), pos.z());
}

bool Grid::testCellInGrid(int x, int y, int z)
{
    return x >= 0 && x < nx &&
           y >= 0 && y < ny &&
           z >= 0 && z < nz;
}

int Grid::getCellIdx(int x, int y, int z)
{
    return x * ny * nz + y * nz + z;
}

int Grid::getCellIdx(const Vec3f &pos)
{
    return getCellIdx(pos.x(), pos.y(), pos.z());
}

bool Grid::testCellInGrid(const Vec3f &pos)
{
    return testCellInGrid(pos.x(), pos.y(), pos.z());
}

void MarchingInfo::nextCell()
{
    if (!intersected && grid->getCellObjects(i, j, k))
    {
        intersected = true;
        intersected_t = t;
        intersected_mat = grid->getCellMat(i, j, k);
        intersected_normal = normal;
    }

    BoundingBox bb = grid->getCellBox(i, j, k);
    Vec3f tmax_axes = getTMax(bb, ray);
    float tmin = fmin(tmax_axes.x(), fmin(tmax_axes.y(), tmax_axes.z()));
    normal = getTNormal(tmin, tmax_axes, ray.getDirection());

    i += -normal.x();
    j += -normal.y();
    k += -normal.z();
}

Vec3f MarchingInfo::getCell()
{
    return Vec3f(i, j, k);
}

const Vec3f &MarchingInfo::getNormal()
{
    return normal;
}

bool MarchingInfo::isIntersected()
{
    return intersected;
}

const Vec3f &MarchingInfo::getIntersectedNormal()
{
    return intersected_normal;
}

float MarchingInfo::getIntersectedT()
{
    return intersected_t;
}

Material *MarchingInfo::getIntersectedMat()
{
    return intersected_mat;
}

void MarchingInfo::setCellPos(int _i, int _j, int _k)
{
    i = _i;
    j = _j;
    k = _k;
}

void MarchingInfo::setCellPos(const Vec3f &pos)
{
    setCellPos(pos.x(), pos.y(), pos.z());
}

void MarchingInfo::setGrid(Grid *g)
{
    grid = g;
}

void MarchingInfo::setRay(const Ray &r)
{
    ray = r;
    const Vec3f &d = ray.getDirection();
}

void MarchingInfo::setNormal(const Vec3f &n)
{
    normal = n;
}

void MarchingInfo::setT(float _t)
{
    t = _t;
}
