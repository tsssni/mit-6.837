# MIT 6.837 Assignments

![mit 6.837](https://i1.lensdump.com/i/TBxLPi.png)

Assignments of [MIT 6.837 Intro to Computer Graphics](https://groups.csail.mit.edu/graphics/classes/6.837/F04/calendar.html), finished on March 10, 2023.

---
## Development Environment
- Language: C++20 (with C++ 98 programming style)
- Platform: Ubuntu 22.04.2 LTS (WSL2)
- Toolchain: gcc-11+VSCode+XMake

## File Structure
Each folder corresponds to each assignment. In each assignment foler, source codes are included in **src** folder, input files for the program to read are included in **input** folder, models with .obj format are included in **data** folder, and the outputs are saved in **output** folder.

## Compile
Xmake is the make tool used for the assignments. So instead of the Makefile provided by the course, **xmake.lua** file is provided in each assignment folder. Make sure XMake and OpenGL environment are installed before compilation.

## Run
A shell script file (raytracer.sh e.g.) is provided in each assignment folder. When running on Linux, you can modify the inputs of the program in this file and type commands in shell (bash ./raytracer.sh e.g.) to launch the program.

The path of the compiled program will be influenced by the compile options and xmake.lua file, so make sure the program path in the script file is correct before running.

---

## Sample Results

Markdown does not support to show .tga files, so the output pictures are converted to .png format.

The options for producing the samples are provided.

Pictures of the samples were uploaded to a [picture bed](https://lensdump.com/102103), so stay connected to Internet to display the pictures.

### Assignment0: Iterated Function Systems

-input input/giant_x.txt 
-points 50000 
-iters 30 
-size 400 
-output output/dragon.tga

![dragon](https://i1.lensdump.com/i/TKW0lb.png)

### Assignment1: Ray Casting

-input input/scene1_04.txt 
-size 200 200 
-output output/output1_04.tga 
-depth 12 17 output/depth1_04.tga

![output1_04](https://i2.lensdump.com/i/TKWIb7.png)
![depth1_04](https://i3.lensdump.com/i/TKWgtr.png)

### Assignment2: Transformations & Additional Primitives

-input input/scene2_06_plane.txt 
-size 200 200 
-output output/output2_06.tga 
-depth 8 20 output/depth2_06.tga 
-normals output/normals2_06.tga

![output2_06](https://i2.lensdump.com/i/TKWRj0.png)
![depth2_06](https://i2.lensdump.com/i/TKWr4F.png)
![normals2_06](https://i.lensdump.com/i/TKWtP3.png)

-input input/scene2_10_bunny_1k.txt 
-size 200 200 
-output output/output2_10.tga

![output2_10](https://i2.lensdump.com/i/TKWEMq.png)

-input input/scene2_13_rotated_squashed_sphere.txt
-size 200 200 
-output output/output2_13.tga 
-normals output/normals2_13.tga

![output2_13](https://i.lensdump.com/i/TKWkhA.png)
![normals2_13.png](https://i2.lensdump.com/i/TKWC6M.png)

### Assignment3: OpenGL & Phong Shading

-input input/scene3_08_sphere.txt 
-size 200 200 
-output output/output3_08.tga 
-gui 
-tessellation 20 10 
-gouraud

![3_08 gui](https://i1.lensdump.com/i/TKWJzQ.png)
![output3_08](https://i2.lensdump.com/i/TKWQba.png)

-input input/scene3_09_exponent_variations.txt 
-size 300 300 
-output output/output3_09.tga 
-gui 
-tessellation 100 50 
-gouraud

![3_09 gui](https://i3.lensdump.com/i/TKW9te.png)
![output3_09](https://i2.lensdump.com/i/TKWdKk.png)

### Assignment4: Shadows, Reflection & Refraction

-input input/scene4_06_transparent_bars.txt 
-size 200 200 
-output output/output4_06f.tga 
-shadows 
-bounces 5 
-weight 0.01 
-shade_back 
-gui

![4_06f gui](https://i2.lensdump.com/i/TKWOwm.png)
![output4_06f](https://i.lensdump.com/i/TKWl1i.png)

-input input/scene4_09_transparent_sphere_2.0.txt
-size 200 200 
-output output/output4_09.tga 
-shadows 
-bounces 5 
-weight 0.01 
-shade_back 
-gui 
-tessellation 30 15

![4_09 gui](https://i2.lensdump.com/i/TKWzKo.png)
![output4_09](https://i3.lensdump.com/i/TKWvqv.png)

-input input/scene4_14_faceted_gem.txt 
-size 200 200 
-output output/output4_14f.tga 
-shadows 
-shade_back 
-bounces 5 
-weight 0.01 

![4_14f gui](https://i2.lensdump.com/i/TKWuST.png)
![output4_14f](https://i3.lensdump.com/i/TKWc03.png)

### Assignment5: Voxel Rendering

-input input/scene5_09_bunny_mesh_40k.txt 
-size 200 200 
-output output/output5_09.tga 
-gui 
-grid 40 40 33 
-visualize_grid

![5_09 gui0](https://i1.lensdump.com/i/TKW5SM.png)
![5_09 gui1](https://i3.lensdump.com/i/TKWh1a.png)
![5_09 gui2](https://i.lensdump.com/i/TKi2pe.png)
![output5_09](https://i1.lensdump.com/i/TKi8Fk.png)

### Assignment6: Grid Acceleration & Solid Textures

-input input/scene6_12_faceted_gem.txt 
-size 200 200 
-output output/output6_12b.tga 
-shadows 
-shade_back 
-bounces 5 
-weight 0.01 
-grid 20 20 20 
-stats

![output6_12b](https://i2.lensdump.com/i/TKiW0x.png)

-input input/scene6_12_faceted_gem.txt 
-size 200 200 
-output output/output6_12c.tga 
-grid 20 20 20 
-visualize_grid

![output6_12a](https://i1.lensdump.com/i/TKiiBH.png)

-input input/scene6_17_marble_vase.txt 
-size 300 300 
-output output/output6_17a.tga 
-grid 15 30 15 
-bounces 1 
-shadows

![output6_17a](https://i.lensdump.com/i/TKijo1.png)

-input input/scene6_17_marble_vase.txt 
-size 300 300 
-output output/output6_17b.tga 
-grid 15 30 15 
-visualize_grid

![output6_17b](https://i2.lensdump.com/i/TKiI2c.png)

### Assignment7: Supersampling & Antialiasing

-input input/scene7_05_glass_sphere.txt 
-size 300 300 
-output output7_05a.tga 
-shadows 
-shade_back 
-bounces 5 
-weight 0.01 
-grid 20 20 20

![output7_05a](https://i.lensdump.com/i/TKiVJ5.png)

-input input/scene7_05_glass_sphere.txt 
-size 300 300 
-output output/output7_05b.tga 
-shadows 
-shade_back 
-bounces 5 
-weight 0.01 
-grid 20 20 20 
-jittered_samples 4 
-gaussian_filter 0.4

![output7_05b](https://i1.lensdump.com/i/TKi9ez.png)

-input input/scene7_05_glass_sphere.txt 
-size 300 300 
-output output/output7_05c.tga 
-shadows 
-shade_back 
-bounces 5 
-weight 0.01 
-grid 20 20 20 
-jittered_samples 16 
-gaussian_filter 0.4

![output7_05c.png](https://i3.lensdump.com/i/TKidyT.png)

### Assignment8: Curves & Surfaces

-input input/scene8_10_transparent_vase.txt 
-output output/output8_10.tga 
-grid 30 30 30 
-size 300 300 
-bounces 4 
-shade_back 
-jittered_samples 9 
-tent_filter 1.0 
-shadows

![output8_10](https://i2.lensdump.com/i/TKiBRb.png)

-input input/scene8_11_reflective_teapot.txt 
-output output/output8_11.tga 
-grid 50 30 30 
-size 300 300 
-bounces 4 
-shade_back 
-jittered_samples 9 
-tent_filter 1.0 
-shadows

![output8_11](https://i1.lensdump.com/i/TKim77.png)

### Assignment 9: Particle System

-input input/system9_03_hose_force.txt 
-refresh 0.05 
-dt 0.05 
-motion_blur

![system9_03](https://i.lensdump.com/i/TKisUr.png)

-input input/system9_06_circle_rungekutta.txt 
-refresh 0.1  
-dt 0.1  
-integrator_color 
-draw_vectors 0.02

![system9_06](https://i3.lensdump.com/i/TKi4IF.png)

-input input/system9_08_fire.txt 
-refresh 0.05 
-dt 0.05  
-motion_blur

![system9_08](https://i1.lensdump.com/i/TKiKm3.png)