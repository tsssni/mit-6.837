#include "scene_parser.h"
#include "image.h"
#include "group.h"
#include "camera.h"
#include "hit.h"
#include "material.h"
#include "light.h"
#include "glCanvas.h"
#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <math.h>

char *input_file = NULL;
int width = 100;
int height = 100;
char *output_file = NULL;
int tess_phi;
int tess_theta;
bool gui;
bool gouroud;

SceneParser *scene;
GLCanvas *gl_canvas;

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
        else if (!strcmp(argv[i], "-gui"))
        {
            gui = true;
        }
        else if (!strcmp(argv[i], "-gouroud"))
        {
            gouroud = true;
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
    Image outputImg(width, height);

    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            Vec2f point(1.f * i / width, 1.f * j / width);
            Ray r = scene->getCamera()->generateRay(point);
            Hit h(FLT_MAX, nullptr, {0.f, 0.f, 0.f});

            if (scene->getGroup()->intersect(r, h, scene->getCamera()->getTMin()))
            {
                if (-1.f * r.getDirection().Dot3(h.getNormal()) > 0.f)
                {
                    Vec3f color = scene->getAmbientLight() * h.getMaterial()->getDiffuseColor();

                    for (int i = 0; i < scene->getNumLights(); ++i)
                    {
                        Vec3f dir;
                        Vec3f col;
                        float dist;
                        scene->getLight(i)->getIllumination({}, dir, col, dist);

                        color += h.getMaterial()->Shade(r, h, dir, col);
                    }

                    outputImg.SetPixel(i, j, color);
                }
            }
            else
            {
                outputImg.SetPixel(i, j, scene->getBackgroundColor());
            }
        }
    }

    outputImg.SaveTGA(output_file);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    parse(argc, argv);
    scene = new SceneParser(input_file);

    if (gui)
    {
        gl_canvas = new GLCanvas();
        gl_canvas->initialize(scene, Render);
    }
    else if(output_file)
    {
        Render();
    }

    if (gl_canvas)
    {
        delete gl_canvas;
    }

    if (scene)
    {
        delete scene;
    }

    return 0;
}
