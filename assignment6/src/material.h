#ifndef _MATERIAL_H_
#define _MATERIAL_H_
#include "vectors.h"

class Ray;
class Hit;

class Material
{

public:
    Material(const Vec3f &d_color) { diffuseColor = d_color; }
    virtual ~Material() {}

    virtual Vec3f getDiffuseColor(const Vec3f& pos) const = 0;
    virtual Vec3f getSpecularColor(const Vec3f& pos) const = 0;
    virtual Vec3f getReflectiveColor(const Vec3f& pos) const = 0;
    virtual Vec3f getTransparentColor(const Vec3f& pos) const = 0;
    virtual float getIndexOfRefraction(const Vec3f& pos) const = 0;

    virtual Vec3f Shade(const Ray &ray,
                        const Hit &hit,
                        const Vec3f &dirToLight,
                        const Vec3f &lightColor) const = 0;
    virtual void glSetMaterial(void) const = 0;

protected:
    Vec3f diffuseColor;
};

class PhongMaterial : public Material
{
public:
    PhongMaterial(const Vec3f &d_color,
                  const Vec3f &s_color,
                  float e,
                  const Vec3f &r_color,
                  const Vec3f &t_color,
                  float idx);

    virtual Vec3f getDiffuseColor(const Vec3f& pos) const override;
    virtual Vec3f getSpecularColor(const Vec3f& pos) const override;
    virtual Vec3f getReflectiveColor(const Vec3f& pos) const override;
    virtual Vec3f getTransparentColor(const Vec3f& pos) const override;
    virtual float getIndexOfRefraction(const Vec3f& pos) const override;

    virtual Vec3f Shade(const Ray &ray,
                        const Hit &hit,
                        const Vec3f &dirToLight,
                        const Vec3f &lightColor) const override;
    virtual void glSetMaterial(void) const override;

protected:

    Vec3f specularColor;
    float exponent;
    Vec3f reflectiveColor;
    Vec3f transparentColor;
    float indexOfRefraction;
};

class Checkerboard : public Material
{
public:
    Checkerboard(Matrix *m, Material *_mat1, Material *_mat2);
    ~Checkerboard();

    virtual Vec3f getDiffuseColor(const Vec3f& pos) const override;
    virtual Vec3f getSpecularColor(const Vec3f& pos) const override;
    virtual Vec3f getReflectiveColor(const Vec3f& pos) const override;
    virtual Vec3f getTransparentColor(const Vec3f& pos) const override;
    virtual float getIndexOfRefraction(const Vec3f& pos) const override;

    virtual Vec3f Shade(const Ray &ray,
                        const Hit &hit,
                        const Vec3f &dirToLight,
                        const Vec3f &lightColor) const override;
    virtual void glSetMaterial(void) const override;

protected:
    bool getMat(const Vec3f &pos) const;

    Matrix *transform;
    Material *mat1;
    Material *mat2;
};

class Noise : public Material
{
public:
    Noise(Matrix *m, Material *_mat1, Material *_mat2, int _octaves);
    ~Noise();

    virtual Vec3f getDiffuseColor(const Vec3f& pos) const override;
    virtual Vec3f getSpecularColor(const Vec3f& pos) const override;
    virtual Vec3f getReflectiveColor(const Vec3f& pos) const override;
    virtual Vec3f getTransparentColor(const Vec3f& pos) const override;
    virtual float getIndexOfRefraction(const Vec3f& pos) const override;

    virtual Vec3f Shade(const Ray &ray,
                        const Hit &hit,
                        const Vec3f &dirToLight,
                        const Vec3f &lightColor) const override;
    virtual void glSetMaterial(void) const override;

protected:
    virtual float getNoise(const Vec3f& pos) const;
    Vec3f interpolate(float noise, const Vec3f &val1, const Vec3f &val2) const;
    float interpolate(float noise, float val1, float val2) const;

    Matrix *transform;
    Material *mat1;
    Material *mat2;
    int octaves;
};

class Marble : public Noise
{
public:
    Marble(Matrix *m, Material *mat1, Material *mat2, int octaves, float _frequency, float _amplitude);
protected:

    virtual float getNoise(const Vec3f& pos) const;

    float frequency;
    float amplitude;
};

class Wood : public Noise
{
public:
    Wood(Matrix *m, Material *mat1, Material *mat2, int octaves, float _frequency, float _amplitude);
protected:

    virtual float getNoise(const Vec3f& pos) const;

    float frequency;
    float amplitude;
};

#endif
