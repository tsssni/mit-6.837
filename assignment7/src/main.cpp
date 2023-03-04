#include "scene_parser.h"
#include "image.h"
#include "group.h"
#include "camera.h"
#include "hit.h"
#include "material.h"
#include "light.h"
#include "glCanvas.h"
#include "raytracer.h"
#include "ray_tree.h"
#include "grid.h"
#include "matrix.h"
#include "raytracing_stats.h"
#include "sample.h"
#include "film.h"
#include "filter.h"
#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <math.h>

char *input_file = NULL;
int width = 100;
int height = 100;
char *output_file = NULL;
int tess_phi = 1;
int tess_theta = 1;
bool shade_back = false;
bool gui = false;
bool gouraud = false;
bool shadows = false;
int bounces = 0;
float weight = 0.01f;
bool grid = false;
int nx = 0;
int ny = 0;
int nz = 0;
bool visualize_grid = false;
bool stats;
int num_samples;
char *samples_file;
int sample_zoom;
float sigma;
char *filter_file;
int filter_zoom;

SceneParser *scene = nullptr;
Grid *scene_grid = nullptr;
GLCanvas *gl_canvas = nullptr;
RayTracer *ray_tracer = nullptr;
Sampler *sampler = nullptr;
Filter *filter = nullptr;

void parse(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-input"))
        {
            i++;
            assert(i < argc);
            input_file = argv[i];
        }
        else if (!strcmp(argv[i], "-size"))
        {
            i++;
            assert(i < argc);
            width = atoi(argv[i]);
            i++;
            assert(i < argc);
            height = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-output"))
        {
            i++;
            assert(i < argc);
            output_file = argv[i];
        }
        else if (!strcmp(argv[i], "-tessellation"))
        {
            i++;
            assert(i < argc);
            tess_theta = atoi(argv[i]);
            i++;
            assert(i < argc);
            tess_phi = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-shade_back"))
        {
            shade_back = true;
        }
        else if (!strcmp(argv[i], "-gui"))
        {
            gui = true;
        }
        else if (!strcmp(argv[i], "-gouraud"))
        {
            gouraud = true;
        }
        else if (!strcmp(argv[i], "-shadows"))
        {
            shadows = true;
        }
        else if (!strcmp(argv[i], "-bounces"))
        {
            ++i;
            assert(i < argc);
            bounces = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-weight"))
        {
            ++i;
            assert(i < argc);
            weight = atof(argv[i]);
        }
        else if (!strcmp(argv[i], "-grid"))
        {
            grid = true;
            ++i;
            assert(i < argc);
            nx = atoi(argv[i]);
            ++i;
            assert(i < argc);
            ny = atoi(argv[i]);
            ++i;
            assert(i < argc);
            nz = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-visualize_grid"))
        {
            visualize_grid = true;
        }
        else if (!strcmp(argv[i], "-stats"))
        {
            stats = true;
        }
        else if (!strcmp(argv[i], "-random_samples"))
        {
            ++i;
            assert(i < argc);
            num_samples = atoi(argv[i]);
            sampler = new RandomSampler(num_samples);
        }
        else if (!strcmp(argv[i], "-uniform_samples"))
        {
            ++i;
            assert(i < argc);
            num_samples = atoi(argv[i]);
            sampler = new UniformSampler(num_samples);
        }
        else if (!strcmp(argv[i], "-jittered_samples"))
        {
            ++i;
            assert(i < argc);
            num_samples = atoi(argv[i]);
            sampler = new JitteredSampler(num_samples);
        }
        else if (!strcmp(argv[i], "-render_samples"))
        {
            ++i;
            assert(i < argc);
            samples_file = argv[i];
            ++i;
            assert(i < argc);
            sample_zoom = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-box_filter"))
        {
            ++i;
            assert(i < argc);
            float radius = atoi(argv[i]);
            filter = new BoxFilter(radius);
        }
        else if (!strcmp(argv[i], "-tent_filter"))
        {
            ++i;
            assert(i < argc);
            float radius = atoi(argv[i]);
            filter = new TentFilter(radius);
        }
        else if (!strcmp(argv[i], "-gaussian_filter"))
        {
            ++i;
            assert(i < argc);
            float sigma = atoi(argv[i]);
            filter = new GaussianFilter(sigma);
        }
        else if (!strcmp(argv[i], "-render_filter"))
        {
            ++i;
            assert(i < argc);
            filter_file = argv[i];
            ++i;
            assert(i < argc);
            filter_zoom = atoi(argv[i]);
        }
        else
        {
            printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
            assert(0);
        }
    }
}

void Render()
{
    if (!output_file && !samples_file)
    {
        return;
    }

    Image image(width, height);
    Film film(width, height, num_samples);
    int size = fmax(width, height);
    int start_i = 0;
    int start_j = 0;

    if (width == size)
    {
        start_j = (width >> 1) - (height >> 1);
    }
    else
    {
        start_i = (height >> 1) - (width >> 1);
    }

    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            for (int k = 0; k < num_samples; ++k)
            {
                Vec2f sample_pos = sampler->getSamplePosition(k);
                Vec2f point((i + start_i + sample_pos.x()) / size, (j + start_j + sample_pos.y()) / size);
                Ray r = scene->getCamera()->generateRay(point);
                Hit h(FLT_MAX, nullptr, {0.f, 0.f, 0.f});

                film.setSample(i, j, k, sample_pos, ray_tracer->traceRay(r, scene->getCamera()->getTMin(), 0, 1.f, 1.f, h));
            }
        }
    }

    if (output_file)
    {
        for (int i = 0; i < width; ++i)
        {
            for (int j = 0; j < height; ++j)
            {
                filter->getColor(i, j, &film);
            }
        }
    }

    if (output_file)
    {
        image.SaveTGA(output_file);
    }

    if (samples_file)
    {
        film.renderSamples(samples_file, sample_zoom);
    }

    if (filter_file)
    {
        film.renderFilter(filter_file, filter_zoom, filter);
    }

    if (stats)
    {
        RayTracingStats::PrintStatistics();
    }
}

void RayTrace(float x, float y)
{
    Ray r = scene->getCamera()->generateRay({x, y});
    Hit h(FLT_MAX, nullptr, {0.f, 0.f, 0.f});

    ray_tracer->traceRay(r, scene->getCamera()->getTMin(), 0, 1.f, 1.f, h);
    RayTree::SetMainSegment(r, scene->getCamera()->getTMin(), h.getT());

    if (visualize_grid)
    {
        scene_grid->intersect(r, h, FLT_MAX);
    }
}

int main(int argc, char **argv)
{
    parse(argc, argv);

    if (gui)
    {
        glutInit(&argc, argv);
    }

    scene = new SceneParser(input_file);

    if (stats)
    {
        RayTracingStats::Initialize(width, height, scene->getGroup()->getBoundingBox(), nx, ny, nz);
    }

    if (nx && ny && nz)
    {
        scene_grid = new Grid(scene->getGroup()->getBoundingBox(), nx, ny, nz);
        Matrix identity;
        identity.SetToIdentity();
        scene->getGroup()->insertIntoGrid(scene_grid, &identity);
    }

    if (!sampler)
    {
        sampler = new UniformSampler(1);
    }

    if (!filter)
    {
        filter = new BoxFilter(1);
    }

    ray_tracer = new RayTracer(scene, scene_grid, bounces, weight, shadows);

    if (gui)
    {
        gl_canvas = new GLCanvas();
        gl_canvas->initialize(scene, Render, RayTrace, scene_grid, visualize_grid);
    }
    else
    {
        Render();
    }

    if (gl_canvas)
    {
        delete gl_canvas;
    }

    if (ray_tracer)
    {
        delete ray_tracer;
    }

    if (scene_grid)
    {
        delete scene_grid;
    }

    if (scene)
    {
        delete scene;
    }

    if (sampler)
    {
        delete sampler;
    }

    if (filter)
    {
        delete filter;
    }

    return 0;
}
