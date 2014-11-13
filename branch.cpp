#include "branch.h"
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include "tree.h"
#include <glm/gtx/string_cast.hpp>
#include <iostream>
using std::cerr;
using std::endl;



void branch::init_matrix() {
    //    model_matrix = glm::mat4();

    //    model_matrix = glm::translate(model_matrix,glm::vec3(-0.5f,1.0f,0.0f));
    //    object::init_matrix();
    //    model_matrix = glm::scale(model_matrix,glm::vec3(0.05f * (1 + w),1.0f * (0.4 + l) ,0.05f * (1 + w)));
    model_matrix = glm::mat4();

    if (m_parent) {
        //        get_absangles();
        //        get_absshift();
        //        glm::vec4 tr_v = m_parent->model_matrix * glm::vec4(0.0f, h, 0.0f, 1.0f);
        //        for (int i = 0; i < 3; i++ ) tr_v[i] = absshift[i];
        //        model_matrix = glm::translate(model_matrix, glm::vec3(tr_v[0], tr_v[1], tr_v[2]));
        //        model_matrix = glm::rotate(model_matrix, abs_angoxz, glm::vec3(0, 0, 1));
        //        model_matrix = glm::rotate(model_matrix, abs_angy, glm::vec3(0, 1, 0));
        object::init_matrix();

    }
    model_matrix = glm::scale(model_matrix, glm::vec3(w,  l,  w));
    model_matrix = glm::scale(model_matrix, glm::vec3(0.05f, 1.0f, 0.05f));
    std::cerr << "LTYME == " << my_ltime <<" MY MATRIX IS \n" << glm::to_string( model_matrix) << endl;
}




void branch::add_child(object *obj) {
    obj->m_parent = this;
    if (obj->getType() == O_BRANCH) {
        if (level_id >= MAX_DEPTH) {
            delete obj;
            return;
        }
        dynamic_cast<branch *>(obj)->set_level(level_id + 1);
    }

    if (childs.size() >= MAX_CAPACITY || (m_parent == NULL && (obj->getType() != O_BRANCH || childs.size() >= 5))) {
        unsigned i = 0;
        while ( i < childs.size() && childs[i]->getType() != O_BRANCH) {
            i++;
        }
        if (i == childs.size()) {
            delete obj;
            return;
        } else {
            childs[i]->add_child(obj);
        }
    } else {
        if (obj->getType() == O_BRANCH && childs.size() == 0) {
            //if (childs.size() == 0) {
            obj->shift = glm::vec3(0, l, 0);
            obj->h = l;
            obj->angleoxz = 0.0;
            obj->angley = 0.0;
            //            }
        } else {
            //obj->shift = glm::vec3(0, (l * (childs.size() + 1.0f)) /  MAX_CAPACITY, 0);
            obj->h = (MAX_CAPACITY - l * (childs.size() + 4.0f)) /  MAX_CAPACITY;
            if (m_parent == NULL && obj->h < 0.1) {
                delete obj;
                return;
            }
            obj->angleoxz =  (rand() %  360 + 1.0f);
            obj->angley = (rand() % 180 + 1.0f);
            obj->angleoxz *= M_PI / 180.0f;
            obj->angley *= M_PI / 180.0f;
        }
        object::add_child(obj);
    }

}

branch::branch(object *parent):object(O_BRANCH, parent)
{
    if (parent == NULL) {
        level_id = 0;
    }
}


branch::~branch(void)
{
}

void branch::draw() {
    init_matrix();
    branchVis *f = Tree::getBranchVis();
    f->build_model();
    Tree::gui_pre_frame(model_matrix, 0);
    if (childs.size() >=0 || m_parent != NULL) {
        //std::cerr << "MY_MATRIX IS \n" << glm::to_string( model_matrix) << endl;
    }
    //    f->visualize();
    //    return;

    f->visualize();
    for (unsigned i = 0; i < childs.size(); i++) {
        childs[i]->draw();
    }


}

void branch::grow()
{
    if (m_parent && fabs(max_l - 1.0f) < 1e-2 && fabs(max_w - 1.0f) < 1e-2) {

        for (int i = 0; i < level_id; i++) {
            max_l *= m_fChildBranchLC;
            max_w *= m_fChildBranchWC;
        }
    }
    object::grow();
}
