#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>

#include <string>

struct Point
{
    float x;
    float y;
    float z;
};

struct Triangle
{
    struct Point P1;
    struct Point P2;
    struct Point P3;
};

struct Plane
{
    struct Point P1;
    struct Point P2;
    struct Point P3;
    struct Point P4;
};

struct Box
{
    float width;
    float height;
    float depth;
    int divs;
};

struct Cone
{
    float radius;
    float height;
    unsigned int slices;
    unsigned int stacks;
};

struct Sphere
{
    float radius;
    unsigned int slices;
    unsigned int stacks;
};

struct Plane init_plane(struct Point, struct Point, struct Point, struct Point);

struct Box init_box(float, float, float, int);

struct Cone init_cone(float, float, unsigned int, unsigned int);

struct Sphere init_sphere(float, unsigned int, unsigned int);

void write_point(FILE *, struct Point);

void write_plane(FILE *, struct Plane);

void write_triangle(FILE *, struct Triangle);

void write_box(FILE *, struct Box);

void write_sphere(FILE *, struct Sphere);

void write_cone(FILE *, struct Cone);

void write_bezier(FILE *, std::string, float);

struct Plane build_plane(float, float);
