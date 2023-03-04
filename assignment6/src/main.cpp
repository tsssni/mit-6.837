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

SceneParser *scene = nullptr;
Grid *scene_grid = nullptr;
GLCanvas *gl_canvas = nullptr;
RayTracer *ray_tracer = nullptr;

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
        else
        {
            printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
            assert(0);
        }
    }
}

void Render()
{
    if (!output_file)
    {
        return;
    }

    Image output_img(width, height);
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
            Vec2f point(1.f * (i + start_i) / size, 1.f * (j + start_j) / size);
            Ray r = scene->getCamera()->generateRay(point);
            Hit h(FLT_MAX, nullptr, {0.f, 0.f, 0.f});

            output_img.SetPixel(i, j, ray_tracer->traceRay(r, scene->getCamera()->getTMin(), 0, 1.f, 1.f, h));
        }
    }

    output_img.SaveTGA(output_file);

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

    return 0;
}
