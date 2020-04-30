#include "generator.h"

struct Point point(float x, float y, float z)
{
    struct Point point;
    point.x = x;
    point.y = y;
    point.z = z;
    return point;
}

struct Triangle init_triangle(struct Point p1, struct Point p2, struct Point p3)
{
    struct Triangle t;
    t.P1 = p1;
    t.P2 = p2;
    t.P3 = p3;
    return t;
}

struct Plane init_plane(struct Point p1, struct Point p2, struct Point p3, struct Point p4)
{
    struct Plane pl;
    pl.P1 = p1;
    pl.P2 = p2;
    pl.P3 = p3;
    pl.P4 = p4;
    return pl;
}

struct Box init_box(float width, float height, float depth, int divisions)
{
    struct Box cube;
    cube.width = width;
    cube.height = height;
    cube.depth = depth;
    cube.divs = divisions;
    return cube;
}

struct Sphere init_sphere(float radius, unsigned int slices, unsigned int stacks)
{
    struct Sphere s;
    s.radius = radius;
    s.slices = slices;
    s.stacks = stacks;
    return s;
}

struct Cone init_cone(float radius, float height, unsigned int slices, unsigned int stacks)
{
    struct Cone c;
    c.radius = radius;
    c.height = height;
    c.slices = slices;
    c.stacks = stacks;
    return c;
}

void multMatrixVector(float *m, float *v, float *res)
{
    for (int j = 0; j < N; ++j)
    {
        res[j] = 0;

        for (int k = 0; k < N; ++k)
        {
            res[j] += v[k] * m[j * N + k];
        }
    }
}

/*void getBezierPoint()
{

}*/

void write_point(FILE *f, struct Point p)
{
    fprintf(f,
            "%a %a %a\n", p.x, p.y, p.z);
}

void write_triangle(FILE *f, struct Triangle t)
{
    write_point(f, t.P1);
    write_point(f, t.P2);
    write_point(f, t.P3);
}

void write_plane(FILE *f, struct Plane pl)
{
    write_triangle(f, init_triangle(pl.P1, pl.P2, pl.P3));
    write_triangle(f, init_triangle(pl.P3, pl.P2, pl.P4));
}

void write_box(FILE *f, struct Box cube)
{
    float x = cube.width / 2;
    float y = cube.height / 2;
    float z = cube.depth / 2;
    int divs = cube.divs;
    float deltaX = cube.width / cube.divs;
    float deltaY = cube.height / cube.divs;
    float deltaZ = cube.depth / cube.divs;

    for (int i = 0; i <= divs; i++)
    {
        float coordx = -x + (i * deltaX);

        if (i == 0)
        {
            for (int j = 0; j < divs; j++)
            {
                float coordy = -y + (j * deltaY);
                float coordy1 = -y + ((j + 1) * deltaY);

                for (int k = 0; k < divs; k++)
                {
                    float coordz = -z + (k * deltaZ);
                    float coordz1 = -z + ((k + 1) * deltaZ);

                    //BACK-LEFT FACES
                    write_triangle(f, init_triangle(
                                          point(coordx, coordy, coordz),
                                          point(coordx, coordy1, coordz1),
                                          point(coordx, coordy1, coordz)));

                    write_triangle(f, init_triangle(
                                          point(coordx, coordy, coordz),
                                          point(coordx, coordy, coordz1),
                                          point(coordx, coordy1, coordz1)));
                }
            }
        }
        else
        {
            if (i == divs)
            {
                for (int j = 0; j < divs; j++)
                {
                    float coordy = -y + (j * deltaY);
                    float coordy1 = -y + ((j + 1) * deltaY);

                    for (int k = 0; k < divs; k++)
                    {
                        float coordz = -z + (k * deltaZ);
                        float coordz1 = -z + ((k + 1) * deltaZ);

                        //FRONT-RIGHT FACES
                        write_triangle(f, init_triangle(
                                              point(coordx, coordy, coordz),
                                              point(coordx, coordy1, coordz),
                                              point(coordx, coordy1, coordz1)));

                        write_triangle(f, init_triangle(
                                              point(coordx, coordy, coordz1),
                                              point(coordx, coordy, coordz),
                                              point(coordx, coordy1, coordz1)));
                    }
                }
            }

            float coordx1 = -x + ((i - 1) * deltaX);

            for (int j = 0; j < divs; j++)
            {
                float coordy = -y + (j * deltaY);
                float coordy1 = -y + ((j + 1) * deltaY);
                float coordz = -z + (j * deltaZ);
                float coordz1 = -z + ((j + 1) * deltaZ);

                //FACES LATERAIS
                //BACK RIGHT FACE
                write_triangle(f, init_triangle(
                                      point(coordx, coordy1, -z),
                                      point(coordx, coordy, -z),
                                      point(coordx1, coordy, -z)));

                write_triangle(f, init_triangle(
                                      point(coordx1, coordy, -z),
                                      point(coordx1, coordy1, -z),
                                      point(coordx, coordy1, -z)));
                //FRONT LEFT FACE
                write_triangle(f, init_triangle(
                                      point(coordx, coordy, z),
                                      point(coordx, coordy1, z),
                                      point(coordx1, coordy, z)));

                write_triangle(f, init_triangle(
                                      point(coordx1, coordy1, z),
                                      point(coordx1, coordy, z),
                                      point(coordx, coordy1, z)));

                //TOP AND BOTTOM FACES
                //BOTTOM FACE
                write_triangle(f, init_triangle(
                                      point(coordx, -y, coordz),
                                      point(coordx, -y, coordz1),
                                      point(coordx1, -y, coordz)));

                write_triangle(f, init_triangle(
                                      point(coordx, -y, coordz1),
                                      point(coordx1, -y, coordz1),
                                      point(coordx1, -y, coordz)));
                //TOP FACE
                write_triangle(f, init_triangle(
                                      point(coordx, y, coordz1),
                                      point(coordx, y, coordz),
                                      point(coordx1, y, coordz)));

                write_triangle(f, init_triangle(
                                      point(coordx1, y, coordz1),
                                      point(coordx, y, coordz1),
                                      point(coordx1, y, coordz)));
            }
        }
    }
}

void write_sphere(FILE *f, struct Sphere sph)
{
    float radius = sph.radius;
    unsigned int slices = sph.slices;
    unsigned int stacks = sph.stacks;

    std::vector<struct Point> verts;

    verts.reserve((slices + 1) * (stacks + 1));
    for (int i = 0; i <= stacks; i++)
    {
        double stack_step = ((double)i) / ((double)stacks) * M_PI;

        for (int j = 0; j <= slices; j++)
        {
            double slice_step = ((double)j) / ((double)slices) * 2 * M_PI;

            float x = radius * cosf(slice_step) * sinf(stack_step);
            float y = radius * cosf(stack_step);
            float z = radius * sinf(slice_step) * sinf(stack_step);

            verts.push_back(point(x, y, z));
        }
    }
    int n = slices * stacks + slices;
    for (int i = 0; i < n; i++)
    {
        struct Point p1 = verts[i + slices];
        struct Point p2 = verts[i];
        struct Point p3 = verts[i + slices + 1];
        struct Point p4 = verts[i + 1];

        write_plane(f, init_plane(p1, p2, p3, p4));
    }
}

void write_cone(FILE *f, struct Cone c)
{
    float radius = c.radius;
    float height = c.height;
    int slices = c.slices;
    int stacks = c.stacks;
    struct Point B_origin = point(0, 0, 0);
    struct Point T_origin = point(0, height, 0);
    const float step = (float)((2 * M_PI) / (float)slices);
    float st = (float)stacks;

    for (unsigned i = 0; i < slices; i++)
    {
        const float I = (float)i;

        const float xi = sin(I * step);
        const float zi = cos(I * step);

        const float xi1 = sin((I + 1) * step);
        const float zi1 = cos((I + 1) * step);

        /* draw top mini cone */
        {
            const float r = radius / st;
            const float h = height * (st - 1) / st; //* (st - 1) / st;

            struct Point P2 = point(r * xi, h, r * zi);
            struct Point P3 = point(r * xi1, h, r * zi1);

            write_triangle(f, init_triangle(T_origin, P2, P3));
        }

        /* draw base */
        {
            struct Point P1 = point(radius * xi, 0, radius * zi);
            struct Point P2 = point(0, 0, 0);
            struct Point P3 = point(radius * xi1, 0, radius * zi1);

            write_triangle(f, init_triangle(P1, P2, P3));
        }

        /* draw side */
        for (unsigned j = 0; j < (stacks - 1); j++)
        {
            const float j1 = (float)j;

            const float y = height * (j1 / stacks);
            const float y1 = height * ((j1 + 1) / stacks);

            const float r = radius * (stacks - j1) / stacks;
            const float r1 = radius * (stacks - j1 - 1) / stacks;

            const struct Point P1 = point(r1 * sin(I * step), y1, r1 * cos(I * step));
            const struct Point P2 = point(r * sin(I * step), y, r * cos(I * step));
            const struct Point P3 = point(r1 * sin((I + 1) * step), y1, r1 * cos((I + 1) * step));
            const struct Point P4 = point(r * sin((I + 1) * step), y, r * cos((I + 1) * step));

            write_plane(f, init_plane(P1, P2, P3, P4));
        }
    }
}

struct Plane build_plane(float width, float height)
{
    float w = width / 2;
    float h = height / 2;

    return init_plane(
        point(-w, 0, -h),
        point(-w, 0, h),
        point(w, 0, -h),
        point(w, 0, h));
}

void read_patch(std::string file, std ::vector<struct Point> *cps, std ::vector<std::vector<int>> *patches)
{
    std::string line;
    std::ifstream infile;
    infile.open(file);

    //Get the number of patches from the first line of file
    getline(infile, line);
    int n_patches = stoi(line);
    patches->reserve(n_patches);

    int k = 0;

    //Get the points for each patch
    for (int i = 0; i < n_patches; i++)
    {
        std::vector<int> patch;
        patch.reserve(16);

        line.clear();
        for (int j = 0; j <= 15; j++)
        {
            infile >> line;
            if (line[line.size() - 1] == ',')
            {
                line = line.substr(0, line.size() - 1);
            }

            patch.push_back(stoi(line));
        }
        patches->push_back(patch);
    }

    //Get the number of control points
    infile >> line;
    int n_controlps = stoi(line);
    cps->reserve(n_controlps);

    //Fill the vector with the control points
    for (int i = 0; i < n_controlps; i++)
    {
        std::string x;
        std::string y;
        std::string z;
        infile >> x;
        x = x.substr(0, x.size() - 1);
        infile >> y;
        y = y.substr(0, y.size() - 1);
        infile >> z;
        cps->push_back(point(std::stof(x), std::stof(y), std::stof(z)));
    }

    infile.close();
}

void write_patch(FILE *out, std ::vector<struct Point> control_ps, std::vector<int> patch, float tess)
{
    const struct Point P[4][4] = {
        {
            control_ps[patch[0]],
            control_ps[patch[1]],
            control_ps[patch[2]],
            control_ps[patch[3]],
        },
        {
            control_ps[patch[4]],
            control_ps[patch[5]],
            control_ps[patch[6]],
            control_ps[patch[7]],
        },
        {
            control_ps[patch[8]],
            control_ps[patch[9]],
            control_ps[patch[10]],
            control_ps[patch[11]],
        },
        {
            control_ps[patch[12]],
            control_ps[patch[13]],
            control_ps[patch[14]],
            control_ps[patch[15]],
        },
    };

    struct Point p = point(0, 0, 0);
    struct Point MPM[4][4];
}

void write_bezier(FILE *out, std::string in, float tess)
{
    std ::vector<std::vector<int>> patches;
    std ::vector<struct Point> control_ps;
    read_patch(in, &control_ps, &patches);
    for (std::vector<int> patch : patches)
        write_patch(out, control_ps, patch, tess);
}