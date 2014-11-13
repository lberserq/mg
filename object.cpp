#include "object.h"
#include "tree.h"
#include "leaf.h"
#include <glm/gtx/string_cast.hpp>
typedef unsigned uint;

#include <cstdio>

object::object(OBJ_TYPE type, object  *parent):
    m_parent(parent),
    m_type(type){
    if (parent == NULL) {
        //        angley = M_PI / 2.0f;
        //        angleoxz = M_PI / 2.0f;
        angley = 0.0f;
        angleoxz = 0.0f;
    }
    my_ltime = Tree::get_world_tick();

    init_matrix();
    if (parent) {
        m_fLength = parent->m_fLength * m_fChildBranchLC;
        m_fWidth = parent->m_fWidth * m_fChildBranchWC;
    } else {
        m_fLength = 0.8;
        m_fWidth = 0.8;
    }

    l = m_fLength;
    w = m_fWidth;
    max_l = 1.0f;
    max_w = 1.0f;
}

//

void object::get_absangles()
{
    abs_angy = abs_angoxz = 0.0f;
    if (this->m_parent != NULL) {
        glm::vec4 direction = glm::vec4(0, 1, 0, 0);
        glm::vec4 coord =  m_parent->model_matrix * direction;
        float x = coord[0];
        float y = coord[1];
        float z = coord[2];
        const double eps = 1e-4;
        if (std::fabs(x) < eps) {
            abs_angoxz = (x >= 0) ? M_PI / 2.0f : -M_PI / 2.0f;
        } else {
            abs_angoxz = static_cast<float>(std::atan2(z, x));
        }
        abs_angy = static_cast<float>(std::acos(y / std::sqrt(x * x + y * y + z * z)));
    }
    abs_angy += angley;
    float tmp1 = abs_angy * 180.0f / M_PI;

    if (std::fabs(abs_angy) > MAX_ANGLE) {
        abs_angy -= angley;
        angley = (angley > 0.0f) ? angley - MAX_ANGLE : angley + MAX_ANGLE;
        abs_angy += angley;
    }


    if ((std::fabs(abs_angy) < MIN_ANGLE && std::fabs(abs_angy) > 1e-2) || abs_angy < 0) {
        abs_angy -= angley;
        angley = (angley >= 0.0f) ? angley + MIN_ANGLE : angley - MIN_ANGLE;
        abs_angy += angley;
    }

    if (abs_angoxz < 0) {
        abs_angoxz += M_PI * 2.0f;
    }

    tmp1 = abs_angy * 180.0f / M_PI;

    abs_angoxz += angleoxz;
    float tmp2 = abs_angoxz * 180.0f / M_PI;/*
    if (abs_angoxz < 0.0f || std::fabs(abs_angoxz) > MAX_ANGLE) {
        abs_angoxz -= angleoxz;
        angleoxz *= -1;
        abs_angoxz += angleoxz;
    }
    tmp2 = abs_angoxz * 180.0f / M_PI;*/
    abs_angoxz -= std::floor(abs_angoxz / (M_PI * 2.0f)) * M_PI * 2.0f;

    const float x = 69;
    if (my_ltime == 140 && fabs(tmp1 - x) < 0.01) {
        my_ltime = 140;
        get_absshift();

    }

    float ty = abs_angy, tx = abs_angoxz;
    abs_angy *= M_PI / 180.0f;
    abs_angoxz *= M_PI / 180.f;
    generate_matrix();


    glm::vec4 tmpv = model_matrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    model_matrix = glm::mat4();

    absshift[0] = tmpv[0];
    absshift[2] = tmpv[2];
    abs_angy = ty, abs_angoxz = tx;



    glm::vec2 projvct(absshift[0], absshift[2]);
    glm::vec2 delt(fcos(abs_angoxz), fsin(abs_angoxz));
    projvct = projvct + delt;
    if (!(fabs(projvct[0])  > fabs(absshift[0]))) {
        abs_angoxz -= angleoxz;
        angleoxz = M_PI  -  angleoxz;
        abs_angoxz += angleoxz;
    }

    if (fabs(projvct[1]) < fabs(absshift[2])) {
        abs_angoxz -= angleoxz;
        angleoxz = - angleoxz;
        abs_angoxz += angleoxz;

    }





    abs_angoxz *= 180.f / M_PI;
    abs_angy *= 180.f / M_PI;

    int gen = 0;
    if (m_type == O_BRANCH) {
        branch *p = dynamic_cast<branch *>(this);
        if (p != NULL) {
            gen = p->level_id;
        }
    }

    std::fprintf(stderr, "%p  GENERATION %d LTIME %d FANGLES %.6f %.6f\n", this, gen, my_ltime, abs_angy, abs_angoxz);
}

void object::get_absshift() {
    absshift = glm::vec3(0, 0, 0);
    if (this->m_parent != NULL) {
        glm::vec4 tmp = m_parent->model_matrix * glm::vec4(0, h, 0, 1);
        for (int i = 0; i < 3; i++) {
            absshift[i] = tmp[i];
        }
    }
    std::fprintf(stderr, "LTIME %d %s\n", my_ltime, glm::to_string(absshift).c_str());
    //absshift += shift;
}


object::~object()
{

}




void object::grow() //this function works only in branch (now) !!!!
{
    int my_time = Tree::get_world_tick() - my_ltime;
    l = m_fLength * (1 + m_fIncLStep * my_time);
    w = m_fWidth * (1 + m_fIncWStep * my_time);

    l = std::min(l, max_l);
    w = std::min(l, max_w);
    for (unsigned i = 0; i < childs.size(); i++) {
        childs[i]->grow();
    }
    if (my_time && my_time % T_NEW == 0) {
        object *obj;
        if (0) {
            obj = new leaf(this);
        } else {
            obj = new branch(this);
            //obj->shift = glm::vec3(0, 0, l);
        }
        fprintf(stderr, "Tick %d new child\n", Tree::get_world_tick());
        add_child(obj);
    }
}

