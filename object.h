#ifndef OBJECT_H
#define OBJECT_H
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <cmath>
#define fsin(x) (static_cast<float> (sin(x)))
#define fcos(x) (static_cast<float> (cos(x)))
enum OBJ_TYPE{
    O_LEAF = 1,
    O_BRANCH = 2,
    O_OTHER = 3

};




class object
{
    friend class branch;
    friend class leaf;
protected:
    glm::vec3 shift;
    float angley;//angle of between obj and oy
    float angleoxz;

    float abs_angy;
    float abs_angoxz;
    glm::vec3 absshift;

    float h; //magic magic


    float l, w;
    float m_fLength;
    float m_fWidth;

    int my_ltime;
    object *m_parent;

    OBJ_TYPE m_type;

    std::vector<object *> childs;
    glm::mat4x4 model_matrix;
    //glm::mat4x4 trans_matr;
    //Matrix my_reprentation;
    virtual void init_matrix() {
        get_absangles();
        get_absshift();
        model_matrix = glm::translate(model_matrix, absshift)
                * glm::rotate(model_matrix, (abs_angy), glm::vec3(0.0f, 1.0f, 0.0f))
                * glm::rotate(model_matrix, (abs_angoxz), glm::vec3(0.0f, 0.0f, 1.0f));
    }

    void get_absangles(); // from parent;
    void get_absshift();

public:

    static glm::mat4 gen_Ry(float fi) {
        glm::mat4 res = glm::mat4();
        res[1][1] = fcos(fi);
        return res;
    }

    static glm::mat4 gen_Rxz(float fi) {

        glm::mat4 res = glm::mat4();
        res[0][0] = fcos(fi);
        res[0][2] = -fsin(fi);
        res[2][0] = -res[2][0];
        res[2][2] = res[0][0];
        return res;
    }


    static glm::mat4 genS(glm::vec3 shf) {
        glm::mat4 res = glm::mat4();
        for (int i = 0; i < 3; i++) {
            res[i][3] = shf[i];
        }
        return res;
    }

    object(OBJ_TYPE type, object  *parent = NULL);

    /*virtual void grow() {
        int my_time = Tree::get_world_tick() - my_ltime;
        l = m_fLength * (1 + m_fIncLStep * my_time);
        w = m_fWidth * (1 + m_fIncWStep * my_time);
    }*/

    virtual void grow();


    virtual void add_child(object *child) {
        child->m_parent = this;
        childs.push_back(child);
    }
    virtual void draw() = 0;


    virtual ~object();
    OBJ_TYPE getType() {
        return m_type;
    }
};

#endif // OBJECT_H
