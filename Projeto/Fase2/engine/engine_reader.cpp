#include "engine_reader.h"

struct Point point(float x, float y, float z)
{
    struct Point point;
    point.x = x;
    point.y = y;
    point.z = z;
    return point;
}

struct gt init_translate()
{
    struct gt t;
    t.p = point(0, 0, 0);
    t.gt_type = TRANSLATE;

    return t;
}

struct gt init_scale(void)
{
    struct gt s;
    s.p = point(1, 1, 1);
    s.gt_type = SCALE;

    return s;
}

struct gt init_rotate(void)
{
    struct gt r;
    r.p = point(0, 0, 0);
    r.gt_type = ROTATE;
    r.r_angle = 0.0;

    return r;
}

//Function that loads the translation of the .xml file
void load_translate(TiXmlElement *transf, struct group *g)
{
    struct gt point = init_translate();

    if (transf->Attribute("X"))
        point.p.x = atof(transf->Attribute("X"));
    if (transf->Attribute("Y"))
        point.p.y = atof(transf->Attribute("Y"));
    if (transf->Attribute("Z"))
        point.p.z = atof(transf->Attribute("Z"));

    g->gt.push_back(point);
}

//Function that loads the scale data from the .xml file
void load_scale(TiXmlElement *transf, struct group *g)
{
    struct gt scale = init_scale();

    if (transf->Attribute("X"))
        scale.p.x = atof(transf->Attribute("X"));
    if (transf->Attribute("Y"))
        scale.p.y = atof(transf->Attribute("Y"));
    if (transf->Attribute("Z"))
        scale.p.z = atof(transf->Attribute("Z"));

    g->gt.push_back(scale);
}

//Function that loads the rotation data from the .xml file
void load_rotate(TiXmlElement *transf, struct group *g)
{
    struct gt rotate = init_rotate();

    if (transf->Attribute("angle"))
        rotate.r_angle = atof(transf->Attribute("angle"));
    if (transf->Attribute("axisX"))
        rotate.p.x = atof(transf->Attribute("axisX"));
    if (transf->Attribute("axisY"))
        rotate.p.y = atof(transf->Attribute("axisY"));
    if (transf->Attribute("axisZ"))
        rotate.p.z = atof(transf->Attribute("axisZ"));

    g->gt.push_back(rotate);
}

//Function that loads the models data from the .xml and subsequently the .3d file
void load_models(TiXmlElement *models, struct group *g)
{
    for (TiXmlElement *model = models->FirstChildElement("model"); model; model = model->NextSiblingElement())
    {
        const char *file = model->Attribute("file");
        if (file)
        {
            string fileDir = "../../assets/";
            fstream f;
            string file3d = fileDir + file;
            f.open(file3d, ios::in);
            if (f.is_open())
            {
                string line;
                vector<struct Point> points;
                getline(f, line);
                while (!line.empty())
                {
                    double x, y, z;

                    sscanf(line.c_str(), "%lf %lf %lf\n", &x, &y, &z);

                    struct Point p = point(x, y, z);
                    points.push_back(p);
                    getline(f, line);
                }
                g->models.push_back(points);
                f.close();
            }
            else
            {
                printf("erro ao abrir o ficheiro .3d!\n");
            }
        }
        else
        {
            printf("Erro no ficheiro xml!!\n");
        }
    }
}

struct group process_groups(TiXmlElement *group, struct scene *scene)
{
    struct group g;

    if (!group)
    {
        return g;
    }

    for (TiXmlElement *childGroup = group->FirstChildElement("group"); childGroup; childGroup = childGroup->NextSiblingElement())
    {
        g.child.push_back(process_groups(childGroup, scene));
    }

    TiXmlElement *trans = group->FirstChildElement("translate");
    if (trans)
    {
        load_translate(trans, &g);
    }

    TiXmlElement *scale = group->FirstChildElement("scale");
    if (scale)
    {
        load_scale(scale, &g);
    }

    TiXmlElement *rotate = group->FirstChildElement("rotate");
    if (rotate)
    {
        load_rotate(rotate, &g);
    }

    TiXmlElement *models = group->FirstChildElement("models");
    if (models)
    {
        scene->nModels++;
        load_models(models, &g);
    }
    return g;
}

void load_scene(struct scene *scene, TiXmlElement *group)
{
    scene->groups.push_back(process_groups(group, scene));
}

void draw_gt(struct group g)
{
    for (int i = 0; i < g.gt.size(); i++)
    {
        struct gt transf = g.gt[i];
        if (transf.gt_type == SCALE)
        {
            glScalef(transf.p.x, transf.p.y, transf.p.z);
        }
        if (transf.gt_type == TRANSLATE)
        {
            glTranslatef(transf.p.x, transf.p.y, transf.p.z);
        }
        if (transf.gt_type == ROTATE)
        {
            glRotatef(transf.r_angle, transf.p.x, transf.p.y, transf.p.z);
        }
    }
}

void draw_models(struct group g)
{
    for (unsigned i = 0; i < g.models.size(); i++)
    {
        vector<struct Point> model = g.models[i];
        for (unsigned j = 0; j < model.size(); j += 3)
        {
            glBegin(GL_TRIANGLES);
            glVertex3f(model[j].x, model[j].y, model[j].z);
            glVertex3f(model[j + 1].x, model[j + 1].y, model[j + 1].z);
            glVertex3f(model[j + 2].x, model[j + 2].y, model[j + 2].z);
            glEnd();
        }
    }
}

/*void draw_scene(vector<struct group> groups)
{
    for (int i = 0; i < groups.size(); i++)
    {
        struct group group = groups[i];
        glPushMatrix();
        {
            draw_gt(group);
            draw_scene(group.child);
            draw_models(group);
        }
        glPopMatrix();
    }
}*/