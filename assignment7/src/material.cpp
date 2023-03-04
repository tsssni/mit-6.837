#include "material.h"
#include "ray.h"
#include "hit.h"
#include "glCanvas.h"
#include "matrix.h"
#include "perlin_noise.h"
#include <GL/gl.h>
#include <algorithm>

#ifdef SPECULAR_FIX
extern int SPECULAR_FIX_WHICH_PASS;
#endif

extern bool shade_back;

PhongMaterial::PhongMaterial(const Vec3f &d_color, const Vec3f &s_color, float e, const Vec3f &r_color, const Vec3f &t_color, float idx)
    : Material(d_color), specularColor(s_color), exponent(e), reflectiveColor(r_color), transparentColor(t_color), indexOfRefraction(idx)
{
}

Vec3f PhongMaterial::getDiffuseColor(const Vec3f &pos) const
{
    return diffuseColor;
}

Vec3f PhongMaterial::getSpecularColor(const Vec3f &pos) const
{
    return specularColor;
}

Vec3f PhongMaterial::getReflectiveColor(const Vec3f &pos) const
{
    return reflectiveColor;
}

Vec3f PhongMaterial::getTransparentColor(const Vec3f &pos) const
{
    return transparentColor;
}

float PhongMaterial::getIndexOfRefraction(const Vec3f &pos) const
{
    return indexOfRefraction;
}

Vec3f PhongMaterial::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const
{
    Vec3f normal = hit.getNormal();
    
    if (shade_back && ray.getDirection().Dot3(normal) > 0.f)
    {
        normal *= -1.f;
    }

    Vec3f diffuse_color = fmax(0.f, dirToLight.Dot3(normal)) * diffuseColor * lightColor;

    Vec3f half = -1.f * ray.getDirection() + dirToLight;
    half.Normalize();
    Vec3f specular_color = powf(fmax(0.f, half.Dot3(normal)), exponent) * specularColor * lightColor;

    return diffuse_color + specular_color;
}

void PhongMaterial::glSetMaterial(void) const
{
    Vec3f pos;

    GLfloat one[4] = {1.0, 1.0, 1.0, 1.0};
    GLfloat zero[4] = {0.0, 0.0, 0.0, 0.0};
    GLfloat specular[4] = {
        getSpecularColor(pos).r(),
        getSpecularColor(pos).g(),
        getSpecularColor(pos).b(),
        1.0};
    GLfloat diffuse[4] = {
        getDiffuseColor(pos).r(),
        getDiffuseColor(pos).g(),
        getDiffuseColor(pos).b(),
        1.0};

    // NOTE: GL uses the Blinn Torrance version of Phong...
    float glexponent = exponent;
    if (glexponent < 0)
        glexponent = 0;
    if (glexponent > 128)
        glexponent = 128;

#if !SPECULAR_FIX

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);

#else

    // OPTIONAL: 3 pass rendering to fix the specular highlight
    // artifact for small specular exponents (wide specular lobe)

    if (SPECULAR_FIX_WHICH_PASS == 0)
    {
        // First pass, draw only the specular highlights
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, zero);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);
    }
    else if (SPECULAR_FIX_WHICH_PASS == 1)
    {
        // Second pass, compute normal dot light
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, one);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
    }
    else
    {
        // Third pass, add ambient & diffuse terms
        assert(SPECULAR_FIX_WHICH_PASS == 2);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
    }

#endif
}

Checkerboard::Checkerboard(Matrix *m, Material *_mat1, Material *_mat2)
    : Material({1.f, 1.f, 1.f}), transform(m), mat1(_mat1), mat2(_mat2)
{
}

Checkerboard::~Checkerboard()
{
    if (transform)
    {
        delete transform;
    }
}

Vec3f Checkerboard::getDiffuseColor(const Vec3f &pos) const
{
    return !getMat(pos) ? mat1->getDiffuseColor(pos) : mat2->getDiffuseColor(pos);
}

Vec3f Checkerboard::getSpecularColor(const Vec3f &pos) const
{
    return !getMat(pos) ? mat1->getSpecularColor(pos) : mat2->getSpecularColor(pos);
}

Vec3f Checkerboard::getReflectiveColor(const Vec3f &pos) const
{
    return !getMat(pos) ? mat1->getReflectiveColor(pos) : mat2->getReflectiveColor(pos);
}

Vec3f Checkerboard::getTransparentColor(const Vec3f &pos) const
{
    return !getMat(pos) ? mat1->getTransparentColor(pos) : mat2->getTransparentColor(pos);
}

float Checkerboard::getIndexOfRefraction(const Vec3f &pos) const
{
    return !getMat(pos) ? mat1->getIndexOfRefraction(pos) : mat2->getIndexOfRefraction(pos);
}

Vec3f Checkerboard::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const
{
    return !getMat(hit.getIntersectionPoint()) ? mat1->Shade(ray, hit, dirToLight, lightColor) : mat2->Shade(ray, hit, dirToLight, lightColor);
}

void Checkerboard::glSetMaterial(void) const
{
    mat1->glSetMaterial();
}

bool Checkerboard::getMat(const Vec3f &pos) const
{
    Vec3f transformed_pos = pos;
    transform->Transform(transformed_pos);

    // use the sum of the interger coordinate to
    // choose the material for shading
    int coord_sum = floorf(transformed_pos[0]) +
                    floorf(transformed_pos[1]) +
                    floorf(transformed_pos[2]);

    return coord_sum % 2;
}

Noise::Noise(Matrix *m, Material *_mat1, Material *_mat2, int _octaves)
    : Material({1.f, 1.f, 1.f}), transform(m), mat1(_mat1), mat2(_mat2), octaves(_octaves)
{
}

Noise::~Noise()
{
    if (transform)
    {
        delete transform;
    }
}

Vec3f Noise::getDiffuseColor(const Vec3f &pos) const
{
    float noise = getNoise(pos);
    return interpolate(noise, mat1->getDiffuseColor(pos), mat2->getDiffuseColor(pos));
}

Vec3f Noise::getSpecularColor(const Vec3f &pos) const
{
    float noise = getNoise(pos);
    return interpolate(noise, mat1->getSpecularColor(pos), mat2->getSpecularColor(pos));
}

Vec3f Noise::getReflectiveColor(const Vec3f &pos) const
{
    float noise = getNoise(pos);
    return interpolate(noise, mat1->getReflectiveColor(pos), mat2->getReflectiveColor(pos));
}

Vec3f Noise::getTransparentColor(const Vec3f &pos) const
{
    float noise = getNoise(pos);
    return interpolate(noise, mat1->getTransparentColor(pos), mat2->getTransparentColor(pos));
}

float Noise::getIndexOfRefraction(const Vec3f &pos) const
{
    float noise = getNoise(pos);
    return interpolate(noise, mat1->getIndexOfRefraction(pos), mat2->getIndexOfRefraction(pos));
}

Vec3f Noise::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const
{
    float noise = getNoise(hit.getIntersectionPoint());
    return interpolate(noise, mat1->Shade(ray, hit, dirToLight, lightColor), mat2->Shade(ray, hit, dirToLight, lightColor));
}

void Noise::glSetMaterial(void) const
{
    mat1->glSetMaterial();
}

float Noise::getNoise(const Vec3f &pos) const
{
    Vec3f transformed_pos = pos;
    transform->Transform(transformed_pos);

    float noise_val = 0.f;
    for (int j = 1; j < 1 << octaves; j <<= 1)
    {
        noise_val += PerlinNoise::noise(transformed_pos.x() * j, transformed_pos.y() * j, transformed_pos.z() * j) / j;
    }

    return noise_val + .5f;
}

Vec3f Noise::interpolate(float noise, const Vec3f &val1, const Vec3f &val2) const
{
    float val[3];
    for (int i = 0; i < 3; ++i)
    {
        val[i] = noise * val1[i] + (1.f - noise) * val2[i];
    }

    return {val[0], val[1], val[2]};
}

float Noise::interpolate(float noise, float val1, float val2) const
{
    return noise * val1 + (1.f - noise) * val2;
}

Marble::Marble(Matrix *m, Material *mat1, Material *mat2, int octaves, float _frequency, float _amplitude)
    : Noise(m, mat1, mat2, octaves), frequency(_frequency), amplitude(_amplitude)
{
}

float Marble::getNoise(const Vec3f &pos) const
{
    Vec3f transformed_pos = pos;
    transform->Transform(transformed_pos);

    float noise_val = 0.f;
    for (int j = 1; j < 1 << octaves; j <<= 1)
    {
        noise_val += PerlinNoise::noise(transformed_pos.x() * j, transformed_pos.y() * j, transformed_pos.z() * j) / j;
    }

    return 0.5f * (1.f + sinf(frequency * transformed_pos.x() + amplitude * noise_val));
}

Wood::Wood(Matrix *m, Material *mat1, Material *mat2, int octaves, float _frequency, float _amplitude)
    : Noise(m, mat1, mat2, octaves), frequency(_frequency), amplitude(_amplitude)
{
}

float Wood::getNoise(const Vec3f &pos) const
{
    Vec3f transformed_pos = pos;
    transform->Transform(transformed_pos);

    float noise_val = 0.f;
    for (int j = 1; j < 1 << octaves; j <<= 1)
    {
        noise_val += PerlinNoise::noise(transformed_pos.x() * j, transformed_pos.y() * j, transformed_pos.z() * j) / j;
    }

    float len = Vec2f(transformed_pos.x(), transformed_pos.z()).Length();
    return 0.5f * (1.f + sinf(frequency * len + amplitude * noise_val));
}
