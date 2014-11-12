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
    model_matrix = glm::mat4();
    model_matrix = glm::translate(model_matrix,glm::vec3(-0.5f,1.0f,0.0f));
    object::init_matrix();
    model_matrix = glm::scale(model_matrix,glm::vec3(0.05f * (1 + w),1.0f * (0.4 + l) ,0.05f * (1 + w)));

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
        if (obj->getType() == O_BRANCH && level_id >= 0) {
            if (childs.size() == 0) {
                obj->shift = glm::vec3(0, l, 0);
                obj->h = l;
                obj->angleoxz = 0.0;
                obj->angley = 0.0;
            }
            branch *p = dynamic_cast<branch *>(obj);
            p->set_level(level_id + 1);
        } else {
            obj->shift = glm::vec3(0, (l * (childs.size() + 1.0f)) /  MAX_CAPACITY, 0);
            obj->h = (l * (childs.size() + 1.0f)) /  MAX_CAPACITY;
            obj->angleoxz = M_PI / 180.0f * (rand() % 30 + 1.0f);
            obj->angley = M_PI / 180.0f * (rand() % 10 + 1.0f);
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
    if (childs.size() || m_parent != NULL) {
        //std::cerr << "MY_MATRIX IS \n" << glm::to_string(trans_matr * model_matrix) << endl;
    }
//    f->visualize();
//    return;
    for (unsigned i = 0; i < childs.size(); i++) {
        childs[i]->draw();
    }

    f->visualize();

}

