#include "scene_parser.h"
#include "image.h"
#include "group.h"
#include "camera.h"
#include "hit.h"
#include "material.h"
#include <stdio.h>
#include <string.h>
#include <float.h>

int main(int argc, char **argv)
{
    char *input_file = NULL;
    int width = 100;
    int height = 100;
    char *output_file = NULL;
    float depth_min = 0;
    float depth_max = 1;
    char *depth_file = NULL;

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
        else
        {
            printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
            assert(0);
        }
    }

    Image outputImg(width, height);
    Image depthImg(width, height);

    SceneParser parser(input_file);
    Group *group = parser.getGroup();
    Camera *camera = parser.getCamera();

    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            Vec2f point(1.f * i / width, 1.f * j / width);
            Hit h(FLT_MAX, nullptr);

            if (group->intersect(camera->generateRay(point), h, camera->getTMin()) && h.getT() >= depth_min && h.getT() < depth_max)
            {
                float depth = 1.f - (h.getT() - depth_min) / (depth_max - depth_min);
                outputImg.SetPixel(i, j, h.getMaterial()->getDiffuseColor());
                depthImg.SetPixel(i, j, Vec3f(depth, depth, depth));
            }
            else
            {
                outputImg.SetPixel(i, j, parser.getBackgroundColor());
            }
        }
    }

    outputImg.SaveTGA(output_file);
    depthImg.SaveTGA(depth_file);
    return 0;
}
