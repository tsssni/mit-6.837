#include "vectors.h"

class SceneParser;
class Ray;
class Hit;
class Grid;

class RayTracer
{
public:
    RayTracer(SceneParser *s, Grid *g, int _max_bounces, float _cutoff_weight, bool _shadows);
    Vec3f traceRay(Ray &ray, float tmin, int bounces, float weight, float index_i, Hit &hit) const;

protected:
    SceneParser *scene;
    Grid *scene_grid;
    int max_bounces;
    float cutoff_weight;
    bool shadows;
};