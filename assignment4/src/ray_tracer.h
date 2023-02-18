#include "vectors.h"

class SceneParser;
class Ray;
class Hit;

class RayTracer
{
public:
    RayTracer(SceneParser *s, int _max_bounces, float _cutoff_weight, bool _shadows);
    Vec3f traceRay(Ray &ray, float tmin, int bounces, float weight, float index_i, Hit &hit) const;
protected:
    SceneParser *scene;
    int max_bounces;
    float cutoff_weight;
    bool shadows;
};