#include "scene_parser.h"
#include "image.h"
#include "group.h"
#include "camera.h"
#include "hit.h"
#include "material.h"
#include "light.h"
#include "glCanvas.h"
#include "ray_tracer.h"
#include "ray_tree.h"
#include "grid.h"
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
int nx = 0;
int ny = 0;
int nz = 0;
bool visualize_grid = false;

SceneParser *scene = nullptr;
Grid *grid = nullptr;
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
        else if(!strcmp(argv[i],"-visualize_grid"))
        {
            visualize_grid = true;
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

    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            Vec2f point(1.f * i / width, 1.f * j / width);
            Ray r = scene->getCamera()->generateRay(point);
            Hit h(FLT_MAX, nullptr, {0.f, 0.f, 0.f});
            
            output_img.SetPixel(i, j, ray_tracer->traceRay(r, scene->getCamera()->getTMin(), 0, 1.f, 1.f, h));
        }
    }

    output_img.SaveTGA(output_file);
}

void RayTrace(float x, float y)
{
    Ray r = scene->getCamera()->generateRay({x, y});
    Hit h(FLT_MAX, nullptr, {0.f, 0.f, 0.f});

    ray_tracer->traceRay(r, scene->getCamera()->getTMin(), 0, 1.f, 1.f, h);
    RayTree::SetMainSegment(r, scene->getCamera()->getTMin(), h.getT());

    grid->intersect(r, h, FLT_MAX);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    parse(argc, argv);

    scene = new SceneParser(input_file);

    if (nx && ny && nz)
    {
        grid = new Grid(scene->getGroup()->getBoundingBox(), nx, ny, nz);
        scene->getGroup()->insertIntoGrid(grid, nullptr);
    }

    ray_tracer = new RayTracer(scene, grid, bounces, weight, shadows);

    if (gui)
    {
        gl_canvas = new GLCanvas();
        gl_canvas->initialize(scene, Render, RayTrace, grid, visualize_grid);
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

    if (grid)
    {
        delete grid;
    }

    if (scene)
    {
        delete scene;
    }

    return 0;
}
