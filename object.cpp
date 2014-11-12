#include "object.h"
#include "tree.h"
#include "leaf.h"
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
        m_fLength = 0.3;
        m_fWidth = 0.01;
    }

    l = m_fLength;
    w = m_fWidth;
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
            abs_angoxz = static_cast<float>(std::atan2(y, x));
        }
        abs_angy = static_cast<float>(std::acos(z / std::sqrt(x * x + y * y + z * z)));
    }
    abs_angy += angley;
    if (abs_angy < 0.0f || std::fabs(abs_angy) > MAX_ANGLE) {
        abs_angy -= angley;
        angley *= -1;
        abs_angy += angley;
    }
    abs_angoxz += angleoxz;

    abs_angoxz *= 180.f / M_PI;
    abs_angy *= 180.f / M_PI;
}

void object::get_absshift() {
    absshift = glm::vec3(0, 0, 0);
    if (this->m_parent != NULL) {
        /*glm::vec4 e = glm::vec4(1, 0, 0, 1);
        absshift[0] = (m_parent->model_matrix *  e)[0];
        e = glm::vec4(0, 1, 0, 1);
        absshift[1] = (m_parent->model_matrix *  e)[1];
        e = glm::vec4(0, 0, 1, 1);
        absshift[2] = (m_parent->model_matrix *  e)[2];*/
        //
        glm::vec4 tmp = m_parent->model_matrix * glm::vec4(0, h, 0, 1);
        for (int i = 0; i < 3; i++) {
            absshift[i] = tmp[i];
        }
    }
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
    for (unsigned i = 0; i < childs.size(); i++) {
        childs[i]->grow();
    }
    if (my_time % T_NEW == 0) {
        object *obj;
        if (my_time % 4 == 3) {
            obj = new leaf(this);
        } else {
            obj = new branch(this);
            //obj->shift = glm::vec3(0, 0, l);
        }
        fprintf(stderr, "Tick %d new child\n", Tree::get_world_tick());
        add_child(obj);
    }
}

