#ifndef TREE_H
#define TREE_H
#include "branch.h"
#include "branchvis.h"
#include "leafvis.h"
//opengl mathematics
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "shaderhelper.h"

#include <string>
//world parameters
const float m_fChildBranchLC = 0.59;
const float m_fChildBranchWC = 0.7;

const int T_GROW = 5;
const int T_NEW = 7;

const float m_fIncLStep = 0.1 * T_NEW;
const float m_fIncWStep = 0.002 * T_NEW;

const double MAX_ANGLE = M_PIl / 180.0  * 70.0;
const double MIN_ANGLE = M_PIl / 180.0 * 10.0;
const int MAX_TICKS = 5 + 4 * T_GROW * T_NEW;

using std::string;
enum
{
    MAX_OBJ_TYPE = 2
};
class Tree //WORLD CLASS
{
    static uint texId[MAX_OBJ_TYPE];
    static leafVis *VLeaf;
    static branchVis *VBranch;
    branch *root;
    static int m_tick;
    static glm::mat4 view_matr;
    static glm::mat4 proj_matr;

    //window size
    static int windowWidth;
    static int windowHeight;

    //last mouse coordinates
    int mouseX,mouseY;

    //camera position
    static glm::vec3 eye;
    //reference point position
    static glm::vec3 cen;
    //up vector direction (head of observer)
    static glm::vec3 up;

    string VertexShaderName = "Vertex.vert";
    string FragmentShaderName = "Fragment.frag";


    static bool  useTexture;

    static ShaderProgram shaderProgram;
    void draw_scene();
  //  Tree();
public:
    /*Tree() {

    }*/
    ~Tree();
    Tree(bool true_run = false);
    static int get_world_tick() {
        return m_tick;
    }

    static branchVis *getBranchVis() {
        return VBranch;
    }

    static leafVis *getLeafVis() {
        return VLeaf;
    }


    static glm::mat4 getViewMatrix() {
        return view_matr;
    }

    static glm::mat4 getProjMatrix() {
        return proj_matr;
    }


    void update();
    void models_init();
    //void gl_init();
    void gui_init();
    static void gui_pre_frame(glm::mat4 ModelMatrix, int texture_num);
    void init_textures();
    static void keyboard(unsigned char key, int mx, int my);
    static void reshape(int width, int height);


};


#endif // TREE_H
