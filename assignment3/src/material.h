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

  virtual Vec3f getDiffuseColor() const { return diffuseColor; }
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
                float e);

  virtual Vec3f getSpecularColor() const;
  virtual Vec3f Shade(const Ray &ray,
                      const Hit &hit,
                      const Vec3f &dirToLight,
                      const Vec3f &lightColor) const override;
  virtual void glSetMaterial(void) const override;

protected:
  Vec3f specularColor;
  float exponent;
};

#endif
