#include "generator.h"

using namespace std;

void plane_help()
{
    printf("Plane->\t\t <outfile> plane <width> <heigth>(optional)\n");
}

void box_help()
{
    printf("Box->\t\t <outfile> box <width> <heigth> <depth> <divisions>(optional)\n");
}

void sphere_help()
{
    printf("Sphere->\t <outfile> sphere <radious> <slices> <stacks>\n");
}

void cone_help()
{
    printf("Cone->\t\t <outfile> cone <radious> <height> <slices> <stacks>\n");
}

void bezier_help()
{
    printf("Bezier->\t <outfile> bezier <tesselation> <infile>\n");
}

void help()
{
    printf("\nHow to create models : \n");
    plane_help();
    box_help();
    sphere_help();
    cone_help();
    bezier_help();
    printf("\n");
}

int prepare_plane_data(int argc, char **argv)
{
    int r = 0;

    if (argc < 4)
    {
        plane_help();
        r = -1;
    }
    else if (argc == 5)
    {
        string dir = "../../assets/";
        string file = dir + argv[1];
        FILE *f = fopen(file.c_str(), "w");

        float height = 0;
        float width = 0;
        sscanf(argv[3], "%f", &width);
        sscanf(argv[4], "%f", &height);

        write_plane(f, build_plane(width, height));

        fclose(f);
    }
    else if (argc == 4)
    {
        string dir = "../../assets/";
        string file = dir + argv[1];
        FILE *f = fopen(file.c_str(), "w");

        float width = 0;
        sscanf(argv[3], "%f", &width);

        float height = width;

        write_plane(f, build_plane(width, height));
    }

    return r;
}

int prepare_box_data(int argc, char **argv)
{
    int r = 0;

    if (argc < 5)
    {
        box_help();
        r = -1;
    }
    else
    {
        string dir = "../../assets/";
        string file = dir + argv[1];
        FILE *f = fopen(file.c_str(), "w");

        float height = 0;
        float width = 0;
        float depth = 0;
        sscanf(argv[3], "%f", &width);
        sscanf(argv[4], "%f", &height);
        sscanf(argv[5], "%f", &depth);
        int divisions = 1;
        if (argc > 6)
        {
            sscanf(argv[6], "%d", &divisions);
        }
        write_box(f, init_box(width, height, depth, divisions));

        fclose(f);
    }
    return r;
}

int prepare_sphere_data(int argc, char **argv)
{
    int r = 0;

    if (argc < 5)
    {
        sphere_help();
        r = -1;
    }
    else
    {
        string dir = "../../assets/";
        string file = dir + argv[1];
        FILE *f = fopen(file.c_str(), "w");

        float radius = 0;
        int slices = 0;
        int stacks = 0;
        sscanf(argv[3], "%f", &radius);
        sscanf(argv[4], "%d", &slices);
        sscanf(argv[5], "%d", &stacks);

        write_sphere(f, init_sphere(radius, slices, stacks));

        fclose(f);
    }

    return r;
}

int prepare_cone_data(int argc, char **argv)
{
    int r = 0;

    if (argc < 7)
    {
        cone_help();
        r = -1;
    }

    else
    {
        string dir = "../../assets/";
        string file = dir + argv[1];
        FILE *f = fopen(file.c_str(), "w");

        float radius = 0;
        float height = 0;
        unsigned int slices = 0;
        unsigned int stacks = 0;

        sscanf(argv[3], "%f", &radius);
        sscanf(argv[4], "%f", &height);
        sscanf(argv[5], "%d", &slices);
        sscanf(argv[6], "%d", &stacks);

        write_cone(f, init_cone(radius, height, slices, stacks));
    }
    return r;
}

int prepare_bezier_data(int argc, char **argv)
{
    int r = 0;

    if (argc < 5)
    {
        bezier_help();
        r = -1;
    }
    else
    {
        string dir = "../../assets/";
        string outfile = dir + argv[1];
        string in = dir + argv[4];
        FILE *out = fopen(outfile.c_str(), "w");
        float tess = 0;
        sscanf(argv[3], "%f", &tess);
        write_bezier(out, in, tess);
        r = 0;
    }

    return r;
}

int handle_options(int argc, char **argv)
{
    int r;

    if (!strcmp(argv[2], "plane"))
    {
        r = prepare_plane_data(argc, argv);
    }
    else if (!strcmp(argv[2], "box"))
    {
        r = prepare_box_data(argc, argv);
    }
    else if (!strcmp(argv[2], "sphere"))
    {
        r = prepare_sphere_data(argc, argv);
    }
    else if (!strcmp(argv[2], "cone"))
    {
        r = prepare_cone_data(argc, argv);
    }
    else if (!strcmp(argv[2], "bezier"))
    {
        r = prepare_bezier_data(argc, argv);
    }
    else
    {
        printf("!!INPUT ERROR!!\n");
        help();
        r = -1;
    }

    return r;
}

int main(int argc, char **argv)
{
    int r = 0;

    if (argc > 2)
    {
        r = handle_options(argc, argv);
    }
    else
    {
        help();
    }
    return r;
}