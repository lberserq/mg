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

//    if (fabs(angleoxz) > M_PI / 2.0f) {
//        fprintf(stderr, "0\n");
//        angleoxz -= (angleoxz > 0)

//        angleoxz = (angleoxz > 0.0f) ? angleoxz - M_PI / 2.0f : angleoxz + M_PI / 2.0f;
//    }

    if (m_parent != NULL) {
        if (std::fabs(angleoxz) < hard_eps && std::fabs(angley) < hard_eps) {
            abs_angy = m_parent->abs_angy;
            abs_angoxz = m_parent->abs_angoxz;
            return;
        } else {
            glm::vec4 direction = glm::vec4(0, 1, 0, 0);
            glm::vec4 coord =  m_parent->model_matrix * direction;
            float x = coord[0];
            float y = coord[1];
            float z = coord[2];
            if (std::fabs(x) < hard_eps) {
                abs_angoxz = (x > 0) ? M_PI / 2.0f : -M_PI / 2.0f;
            } else {
                abs_angoxz = static_cast<float>(std::atan2(z, x));
            }
            //abs_angy = static_cast<float>(std::acos(y / std::sqrt(x * x + y * y + z * z)));
            abs_angy = static_cast<float>(std::atan2(std::sqrt(x * x + z * z), y));
        }
    }
    abs_angy += angley;
    abs_angy -= floor(abs_angy / M_PI) * M_PI;
    float tmp1 = abs_angy * 180.0f / M_PI;

    if (std::fabs(abs_angy) > MAX_ANGLE) {
        abs_angy -= angley;
        fprintf(stderr, "1\n");

        angley = (angley > 0.0f) ? angley - MAX_ANGLE : angley + MAX_ANGLE;
        abs_angy += angley;
    }


    if ((std::fabs(abs_angy) < MIN_ANGLE && std::fabs(abs_angy) > 1e-2) || abs_angy < 0) {
        abs_angy -= angley;
        fprintf(stderr, "2\n");

        angley = (angley >= 0.0f) ? angley + MIN_ANGLE : angley - MIN_ANGLE;
        abs_angy += angley;
    }
    tmp1 = abs_angy * 180.0f / M_PI;
    const float x = 69;
    if (my_ltime == 140 && fabs(tmp1 - x) < 0.01) {
        my_ltime = 140;
        get_absshift();

    }
    abs_angoxz += angleoxz;
    abs_angoxz -= std::floor(abs_angoxz / (M_PI * 2.0f)) * M_PI * 2.0f;
    float tmp2 = abs_angoxz * 180.0f / M_PI;/*
    if (abs_angoxz < 0.0f || std::fabs(abs_angoxz) > MAX_ANGLE) {
        abs_angoxz -= angleoxz;
        angleoxz *= -1;
        abs_angoxz += angleoxz;
    }
    tmp2 = abs_angoxz * 180.0f / M_PI;*/



//    float ty = abs_angy, tx = abs_angoxz;
//    abs_angy *= M_PI / 180.0f;
//    abs_angoxz *= M_PI / 180.f;
//    generate_matrix();


//    //glm::vec4 tmpv = model_matrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
//    //model_matrix = glm::mat4();
//    //    glm::vec4 direct_vector = model_matrix * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
////    absshift[0] = tmpv[0];
////    absshift[2] = tmpv[2];
//    //direct_vector[0] += tmpv[0];
//    //direct_vector[2] += tmpv[2];
//    abs_angy = ty, abs_angoxz = tx;
    get_absshift();







    abs_angoxz *= 180.f / M_PI;
    abs_angy *= 180.f / M_PI;

    int gen = 0;
    if (m_type == O_BRANCH) {
        branch *p = dynamic_cast<branch *>(this);
        if (p != NULL) {
            gen = p->level_id;
        }
    }

    model_matrix = glm::mat4();
    //std::fprintf(stderr, "%p  GENERATION %d LTIME %d FANGLES %.6f %.6f\n", this, gen, my_ltime, abs_angy, abs_angoxz);
}

void object::get_absshift() {
    absshift = glm::vec3(0, 0, 0);
    if (this->m_parent != NULL) {
        glm::vec4 tmp = m_parent->model_matrix * glm::vec4(0, h, 0, 1);
        for (int i = 0; i < 3; i++) {
            absshift[i] = tmp[i];
        }
    }
    //std::fprintf(stderr, "LTIME %d %s\n", my_ltime, glm::to_string(absshift).c_str());
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

