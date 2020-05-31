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

struct Point normalize(struct Point P)
{
    struct Point N;
    float norm = sqrt((P.x * P.x) + (P.y * P.y) + (P.z * P.z));
    N.x = P.x / norm;
    N.y = P.y / norm;
    N.z = P.z / norm;
    return N;
}

struct Point normal(struct Triangle t)
{
    struct Point N = point(0, 0, 0);
    struct Point U = point(t.P2.x - t.P1.x, t.P2.y - t.P1.y, t.P2.z - t.P1.z);
    struct Point V = point(t.P3.x - t.P1.x, t.P3.y - t.P1.y, t.P3.z - t.P1.z);

    N.x = (U.y * V.z) - (U.z * V.y);
    N.y = (U.z * V.x) - (U.x * V.z);
    N.z = (U.x * V.y) - (U.y * V.x);

    return normalize(N);
}

void write_point(FILE *f, struct Point p)
{
    fprintf(f,
            "%a %a %a\n", p.x, p.y, p.z);
}

void write_triangle(FILE *f, struct Triangle t, struct Triangle nt, struct Triangle text)
{
    write_point(f, t.P1);
    write_point(f, nt.P1);
    write_point(f, text.P1);

    write_point(f, t.P2);
    write_point(f, nt.P2);
    write_point(f, text.P2);

    write_point(f, t.P3);
    write_point(f, nt.P3);
    write_point(f, text.P3);
}

void write_plane(FILE *f, struct Plane pl, struct Plane normals, struct Plane textures)
{
    struct Triangle tri1, tri2, nt1, nt2, text1, text2;

    tri1 = init_triangle(pl.P1, pl.P2, pl.P3);
    tri2 = init_triangle(pl.P3, pl.P2, pl.P4);

    nt1 = init_triangle(normals.P1, normals.P2, normals.P3);
    nt2 = init_triangle(normals.P3, normals.P2, normals.P4);

    text1 = init_triangle(textures.P1, textures.P2, textures.P3);
    text2 = init_triangle(textures.P3, textures.P2, textures.P4);

    write_triangle(f, tri1, nt1, text1);
    write_triangle(f, tri2, nt2, text2);
}

void write_plane_data(FILE *f, struct Plane pl)
{
    struct Point N = point(0.0, 1.0, 0.0);
    struct Plane nps = init_plane(N, N, N, N);
    struct Point T1 = point(0, 0, 0);
    struct Point T2 = point(0, 1, 0);
    struct Point T3 = point(1, 0, 0);
    struct Point T4 = point(1, 1, 0);
    write_plane(f, pl, nps, init_plane(T1, T2, T3, T4));
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
    float texFactorX = 1.0f / deltaX;
    float texFactorY = 1.0f / deltaY;
    float texFactorZ = 1.0f / deltaZ;

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

                    struct Point T1 = point(0, 0, 0);
                    struct Point T2 = point(0, 1, 0);
                    struct Point T3 = point(1, 0, 0);
                    struct Point T4 = point(1, 1, 0);

                    Point P1, P2, P3, P4;
                    P1 = point(coordx, coordy, coordz);
                    P2 = point(coordx, coordy, coordz1);
                    P3 = point(coordx, coordy1, coordz);
                    P4 = point(coordx, coordy1, coordz1);

                    //BACK-LEFT FACES
                    struct Point N1 = normal(init_triangle(P1, P4, P3));
                    struct Point N2 = normal(init_triangle(P1, P2, P4));
                    write_triangle(f, init_triangle(P1, P4, P3), init_triangle(N1, N1, N1), init_triangle(T1, T4, T3));
                    write_triangle(f, init_triangle(P1, P2, P4), init_triangle(N2, N2, N2), init_triangle(T1, T2, T4));
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
                        struct Point N = point(1.0, 0.0, 0.0);

                        struct Point T1 = point(0, 0, 0);
                        struct Point T2 = point(0, 1, 0);
                        struct Point T3 = point(1, 0, 0);
                        struct Point T4 = point(1, 1, 0);

                        Point P1, P2, P3, P4;
                        P1 = point(coordx, coordy, coordz);
                        P2 = point(coordx, coordy1, coordz);
                        P3 = point(coordx, coordy, coordz1);
                        P4 = point(coordx, coordy1, coordz1);

                        //FRONT-RIGHT FACES

                        struct Point N1 = normal(init_triangle(P1, P2, P3));
                        struct Point N2 = normal(init_triangle(P3, P2, P4));
                        write_triangle(f, init_triangle(P1, P2, P3), init_triangle(N1, N1, N1), init_triangle(T1, T2, T3));
                        write_triangle(f, init_triangle(P3, P2, P4), init_triangle(N2, N2, N2), init_triangle(T3, T2, T4));
                    }
                }
            }

            float coordx1 = -x + ((i - 1) * deltaX);

            for (int j = 0; j < divs; j++)
            {
                struct Point P1, P2, P3, P4;
                struct Point N;
                struct Plane normals;

                float coordy = -y + (j * deltaY);
                float coordy1 = -y + ((j + 1) * deltaY);
                float coordz = -z + (j * deltaZ);
                float coordz1 = -z + ((j + 1) * deltaZ);

                struct Point T1 = point(0, 0, 0);
                struct Point T2 = point(0, 1, 0);
                struct Point T3 = point(1, 0, 0);
                struct Point T4 = point(1, 1, 0);

                //FACES LATERAIS
                //BACK RIGHT FACE

                P1 = point(coordx, coordy, -z);
                P2 = point(coordx1, coordy, -z);
                P3 = point(coordx, coordy1, -z);
                P4 = point(coordx1, coordy1, -z);

                struct Point N1 = normal(init_triangle(P1, P4, P3));
                struct Point N2 = normal(init_triangle(P3, P2, P4));
                write_triangle(f, init_triangle(P1, P4, P3), init_triangle(N1, N1, N1), init_triangle(T1, T4, T3));
                write_triangle(f, init_triangle(P1, P2, P4), init_triangle(N2, N2, N2), init_triangle(T1, T2, T4));

                //FRONT LEFT FACE

                P1 = point(coordx, coordy, z);
                P2 = point(coordx, coordy1, z);
                P3 = point(coordx1, coordy, z);
                P4 = point(coordx1, coordy1, z);

                N1 = normal(init_triangle(P1, P2, P3));
                N2 = normal(init_triangle(P3, P2, P4));
                write_triangle(f, init_triangle(P1, P2, P3), init_triangle(N1, N1, N1), init_triangle(T1, T2, T3));
                write_triangle(f, init_triangle(P3, P2, P4), init_triangle(N2, N2, N2), init_triangle(T3, T2, T4));

                //TOP AND BOTTOM FACES
                //BOTTOM FACE

                P1 = point(coordx, -y, coordz);
                P2 = point(coordx, -y, coordz1);
                P3 = point(coordx1, -y, coordz);
                P4 = point(coordx1, -y, coordz1);

                N1 = normal(init_triangle(P1, P2, P3));
                N2 = normal(init_triangle(P3, P2, P4));
                write_triangle(f, init_triangle(P1, P2, P3), init_triangle(N1, N1, N1), init_triangle(T1, T2, T3));
                write_triangle(f, init_triangle(P3, P2, P4), init_triangle(N2, N2, N2), init_triangle(T3, T2, T4));

                //TOP FACE

                P1 = point(coordx, y, coordz);
                P2 = point(coordx1, y, coordz);
                P3 = point(coordx, y, coordz1);
                P4 = point(coordx1, y, coordz1);

                N1 = normal(init_triangle(P1, P2, P3));
                N2 = normal(init_triangle(P3, P2, P4));
                write_triangle(f, init_triangle(P1, P2, P3), init_triangle(N1, N1, N1), init_triangle(T1, T2, T3));
                write_triangle(f, init_triangle(P3, P2, P4), init_triangle(N2, N2, N2), init_triangle(T3, T2, T4));
            }
        }
    }
}

void write_sphere(FILE *f, struct Sphere sph)
{
    float radius = sph.radius;
    unsigned int slices = sph.slices;
    unsigned int stacks = sph.stacks;

    std::vector<struct Point> normals;
    std::vector<struct Point> verts;
    std::vector<struct Point> text;

    verts.reserve((slices + 1) * (stacks + 1));
    normals.reserve((slices + 1) * (stacks + 1));
    text.reserve((slices + 1) * (stacks + 1));

    for (int i = 0; i <= stacks; i++)
    {
        double stack_step = ((double)i) / ((double)stacks) * M_PI;

        for (int j = 0; j <= slices; j++)
        {
            double slice_step = ((double)j) / ((double)slices) * 2 * M_PI;

            float nx = cosf(slice_step) * sinf(stack_step);
            float ny = cosf(stack_step);
            float nz = sinf(slice_step) * sinf(stack_step);

            float x = radius * nx;
            float y = radius * ny;
            float z = radius * nz;

            verts.push_back(point(x, y, z));
            normals.push_back(point(nx, ny, nz));
            text.push_back(point(((float)j / (float)slices), ((float)i / (float)stacks), 0));
        }
    }
    int n = slices * stacks + slices;
    for (int i = 0; i < n; i++)
    {
        struct Point p1 = verts[i + slices];
        struct Point p2 = verts[i];
        struct Point p3 = verts[i + slices + 1];
        struct Point p4 = verts[i + 1];

        struct Point n1 = normals[i + slices];
        struct Point n2 = normals[i];
        struct Point n3 = normals[i + slices + 1];
        struct Point n4 = normals[i + 1];

        struct Point t1 = text[i + slices];
        struct Point t2 = text[i];
        struct Point t3 = text[i + slices + 1];
        struct Point t4 = text[i + 1];

        write_plane(f, init_plane(p1, p2, p3, p4), init_plane(n1, n2, n3, n4), init_plane(t1, t2, t3, t4));
        /*struct Point N1 = normal(init_triangle(p1, p2, p3));
        struct Point N2 = normal(init_triangle(p3, p2, p4));
        write_triangle(f, init_triangle(p1, p2, p3), init_triangle(N1, N1, N1));
        write_triangle(f, init_triangle(p3, p2, p4), init_triangle(N2, N2, N2));*/
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
            const float h = height * (st - 1) / st;

            struct Point P2 = point(r * xi, h, r * zi);
            struct Point P3 = point(r * xi1, h, r * zi1);
            struct Point N = normal(init_triangle(T_origin, P2, P3));
            struct Point T1 = point(0, 0, 0);
            struct Point T2 = point(i / slices, i / stacks, 0);
            struct Point T3 = point((i + 1) / stacks, (i + 1) / slices, 0);

            write_triangle(f, init_triangle(T_origin, P2, P3), init_triangle(N, N, N), init_triangle(T2, T2, T2));
        }

        /* draw base */
        {
            struct Point P1 = point(radius * xi, 0, radius * zi);
            struct Point P2 = point(0, 0, 0);
            struct Point P3 = point(radius * xi1, 0, radius * zi1);
            struct Point N = normal(init_triangle(P1, P2, P3));
            struct Point T1 = point(0.5, 0.5, 0);
            struct Point T2 = point(0.5 + 0.2 * xi, 0.5 + 0.2 * zi, 0);
            struct Point T3 = point(0.5 + 0.2 * xi1, 0.5 + 0.2 * zi1, 0);

            write_triangle(f, init_triangle(P1, P2, P3), init_triangle(N, N, N), init_triangle(T1, T2, T3));
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

            struct Point N1 = normal(init_triangle(P1, P2, P3));
            struct Point N2 = normal(init_triangle(P3, P2, P4));

            float step1 = 1 / slices;
            float step2 = 1 / stacks;

            struct Point T1 = point((j1) / slices, (I) / stacks, 0);
            struct Point T2 = point((j1 + step1) / slices, I / stacks, 0);
            struct Point T3 = point((j1) / slices, (I + step2) / stacks, 0);
            struct Point T4 = point((j1 + step1) / slices, (I + step2) / stacks, 0);

            write_triangle(f, init_triangle(P1, P2, P3), init_triangle(N1, N1, N1), init_triangle(T1, T2, T3));
            write_triangle(f, init_triangle(P3, P2, P4), init_triangle(N2, N2, N2), init_triangle(T3, T2, T4));
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

struct Point scalar(float s, struct Point p)
{
    return point(p.x * s, p.y * s, p.z * s);
}

struct Point sum_points(struct Point p, struct Point q)
{
    return point(p.x + q.x, p.y + q.y, p.z + q.z);
}

#define N 4
void transpose(float A[][N], float B[][N])
{
    int i, j;
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            B[i][j] = A[j][i];
}

static void mult_P_Mt(struct Point P[4][4], float M[4][4], struct Point r[4][4])
{
    for (unsigned i = 0; i < 4; i++)
        for (unsigned j = 0; j < 4; j++)
        {
            r[i][j] = point(0, 0, 0);
            for (int k = 0; k < 4; k++)
                r[i][j] = sum_points(r[i][j], scalar(M[k][j], P[i][k]));
        }
}

static void mult_M_P(float M[4][4], struct Point P[4][4], struct Point r[4][4])
{
    for (unsigned i = 0; i < 4; i++)
        for (unsigned j = 0; j < 4; j++)
        {
            r[i][j] = point(0, 0, 0);
            for (unsigned k = 0; k < 4; k++)
                r[i][j] = sum_points(r[i][j], scalar(M[i][k], P[k][j]));
        }
}

static void mult_M_P_Mt(float M[4][4], float Mt[4][4], struct Point P[4][4], struct Point res[4][4])
{
    struct Point aux[4][4];
    mult_M_P(M, P, aux);
    mult_P_Mt(aux, Mt, res);
}

struct Point get_bezier_point(struct Point MPM[4][4], float u, float v)
{
    float t[4] = {u * u * u, u * u, u, 1};
    float tl[4] = {3 * u * u, 2 * u, 1, 0};
    float vv[4] = {v * v * v, v * v, v, 1};

    struct Point tmp[4];

    for (int i = 0; i < 4; i++)
    {
        struct Point aux0 = scalar(tl[0], MPM[i][0]);
        struct Point aux1 = scalar(tl[1], MPM[i][1]);
        struct Point aux2 = scalar(tl[2], MPM[i][2]);
        struct Point aux3 = scalar(tl[3], MPM[i][3]);

        tmp[i] = sum_points(sum_points(sum_points(aux0, aux1), aux2), aux3);
    }

    for (int i = 0; i < 4; i++)
    {
        struct Point aux0 = scalar(t[0], MPM[i][0]);
        struct Point aux1 = scalar(t[1], MPM[i][1]);
        struct Point aux2 = scalar(t[2], MPM[i][2]);
        struct Point aux3 = scalar(t[3], MPM[i][3]);

        tmp[i] = sum_points(sum_points(sum_points(aux0, aux1), aux2), aux3);
    }

    for (int i = 0; i < 4; i++)
    {
        struct Point aux0 = scalar(t[0], MPM[i][0]);
        struct Point aux1 = scalar(t[1], MPM[i][1]);
        struct Point aux2 = scalar(t[2], MPM[i][2]);
        struct Point aux3 = scalar(t[3], MPM[i][3]);

        tmp[i] = sum_points(sum_points(sum_points(aux0, aux1), aux2), aux3);
    }

    struct Point aux0 = scalar(vv[0], tmp[0]);
    struct Point aux1 = scalar(vv[1], tmp[1]);
    struct Point aux2 = scalar(vv[2], tmp[2]);
    struct Point aux3 = scalar(vv[3], tmp[3]);

    return sum_points(sum_points(sum_points(aux0, aux1), aux2), aux3);
}

void gen_single_patch(FILE *out, std::vector<int> patch, std ::vector<struct Point> control_ps, float tess)
{
    //bezier matrix
    float M[4][4] =
        {{
             -1,
             3,
             -3,
             1,
         },
         {
             3,
             -6,
             3,
             0,
         },
         {
             -3,
             3,
             0,
             0,
         },
         {
             1,
             0,
             0,
             0,
         }};

    float Mt[4][4];

    transpose(M, Mt);

    //get the control points for this patch
    struct Point P[4][4] = {
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

    struct Point M_P_Mt[4][4];
    mult_M_P_Mt(M, Mt, P, M_P_Mt);

    for (unsigned i = 1; i <= 4 * tess; i++)
    {
        float u = ((float)i) / (4.0 * tess);
        float ul = ((float)i - 1) / (4.0 * tess);

        for (unsigned j = 1; j <= 4 * tess; j++)
        {
            float v = ((float)j) / (4.0 * tess);
            float vl = ((float)j - 1) / (4.0 * tess);

            struct Point P1 = get_bezier_point(M_P_Mt, u, vl);
            struct Point P2 = get_bezier_point(M_P_Mt, u, v);
            struct Point P3 = get_bezier_point(M_P_Mt, ul, vl);
            struct Point P4 = get_bezier_point(M_P_Mt, ul, v);

            struct Point N1 = normal(init_triangle(P1, P2, P3));
            struct Point N2 = normal(init_triangle(P3, P2, P4));
            write_triangle(out, init_triangle(P1, P2, P3), init_triangle(N1, N1, N1), init_triangle(N1, N1, N1));
            write_triangle(out, init_triangle(P3, P2, P4), init_triangle(N2, N2, N2), init_triangle(N1, N1, N1));
        }
    }
}

void write_bezier(FILE *out, std::string in, float tess)
{
    std ::vector<std::vector<int>> patches;
    std ::vector<struct Point> control_ps;
    read_patch(in, &control_ps, &patches);
    for (std::vector<int> patch : patches)
    {
        gen_single_patch(out, patch, control_ps, tess);
    }
}