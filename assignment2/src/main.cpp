#include "scene_parser.h"
#include "image.h"
#include "group.h"
#include "camera.h"
#include "hit.h"
#include "material.h"
#include "light.h"
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <math.h>

int main(int argc, char **argv)
{
    char *input_file = NULL;
    int width = 100;
    int height = 100;
    char *output_file = NULL;
    float depth_min = 0;
    float depth_max = 1;
    char *depth_file = NULL;
    char *normals_file = NULL;
    bool shade_back = false;

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
        else if (!strcmp(argv[i], "-depth"))
        {
            i++;
            assert(i < argc);
            depth_min = atof(argv[i]);
            i++;
            assert(i < argc);
            depth_max = atof(argv[i]);
            i++;
            assert(i < argc);
            depth_file = argv[i];
        }
        else if (!strcmp(argv[i], "-normals"))
        {
            i++;
            assert(i < argc);
            normals_file = argv[i];
        }
        else if (!strcmp(argv[i], "-shade_back"))
        {
            shade_back = true;
        }
        else
        {
            printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
            assert(0);
        }
    }

    Image outputImg(width, height);
    Image depthImg(width, height);
    Image normalsImg(width, height);

    SceneParser parser(input_file);
    Group *group = parser.getGroup();
    Camera *camera = parser.getCamera();

    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            Vec2f point(1.f * i / width, 1.f * j / width);
            Ray r = camera->generateRay(point);
            Hit h(FLT_MAX, nullptr, {0.f, 0.f, 0.f});
                
            if (group->intersect(r, h, camera->getTMin()))
            {
                float depth_view = fabs((h.getIntersectionPoint() - r.getOrigin()).z());
                float d_view = (-1.f * r.getDirection()).Dot3(h.getNormal());
                Vec3f normal = (shade_back && d_view <= 0) ? -1.f * h.getNormal() : h.getNormal();

                if (shade_back || d_view > 0.f)
                {
                    if (depth_file)
                    {
                        float depth = 1.f - (h.getT() - depth_min) / (depth_max - depth_min);
                        depthImg.SetPixel(i, j, Vec3f(depth, depth, depth));
                    }

                    if (normals_file)
                    {
                        normalsImg.SetPixel(i, j, Vec3f(fabs(normal.x()), fabs(normal.y()), fabs(normal.z())));
                    }

                    if (output_file)
                    {
                        Vec3f color = parser.getAmbientLight() * h.getMaterial()->getDiffuseColor();

                        for (int i = 0; i < parser.getNumLights(); ++i)
                        {
                            Vec3f dir;
                            Vec3f col;
                            parser.getLight(i)->getIllumination({}, dir, col);

                            float d_light = normal.Dot3(dir);
                            if (d_light > 0.f)
                            {
                                color += d_light * col * h.getMaterial()->getDiffuseColor();
                            }
                        }

                        outputImg.SetPixel(i, j, color);
                    }
                }
            }
            else
            {
                outputImg.SetPixel(i, j, parser.getBackgroundColor());
            }
        }
    }

    if (depth_file)
    {
        depthImg.SaveTGA(depth_file);
    }

    if (normals_file)
    {
        normalsImg.SaveTGA(normals_file);
    }

    if (output_file)
    {
        outputImg.SaveTGA(output_file);
    }

    return 0;
}
