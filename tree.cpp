#include "tree.h"
#include "leaf.h"
#include <iostream>
//opengl headers
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtx/string_cast.hpp>
#include <cstdio>
#include <cmath>
#include <stack>

//functions for shader compilation and linking
#include "shaderhelper.h"
int Tree::m_tick;
branchVis *Tree::VBranch;
leafVis *Tree::VLeaf;

glm::mat4 Tree::view_matr;
glm::mat4 Tree::proj_matr;
ShaderProgram Tree::shaderProgram = ShaderProgram();
bool Tree::useTexture = true;
uint Tree::texId[MAX_OBJ_TYPE];
int Tree::windowHeight, Tree::windowWidth;
glm::vec3 Tree::eye;
glm::vec3 Tree::cen;
glm::vec3 Tree::up;
using std::cerr;
using std::endl;

Tree::Tree(bool true_run)
{
    windowWidth = 800;
    windowHeight = 600;
    eye = glm::vec3(0, 0, 10);
    cen = glm::vec3(0.0,0,0);
    up = glm::vec3(0, 1, 0);
    m_tick = 0;
    root  = NULL;
    if (true_run) {
        VBranch = new branchVis();
        VLeaf = new leafVis();


        gui_init();
        models_init();
    }
}

void Tree::models_init() {
    VBranch->build_model();
    VLeaf->build_model();
    VBranch->initGLBuffers(shaderProgram.programObject,"pos","nor","tex");
    VLeaf->initGLBuffers(shaderProgram.programObject,"pos","nor","tex");
    init_textures();
}

void Tree::draw_scene()
{
    glClearColor(0,0,0,0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

      //Draw triangle with shaders (in screen coordinates)
      //need to set uniform in modelViewMatrix

      glUseProgram(shaderProgram.programObject);

      //we will need this uniform locations to connect them to our variables
      int locMV = glGetUniformLocation(shaderProgram.programObject,"modelViewMatrix");
      int locN = glGetUniformLocation(shaderProgram.programObject,"normalMatrix");
      int locP = glGetUniformLocation(shaderProgram.programObject,"modelViewProjectionMatrix");
      int texLoc = glGetUniformLocation(shaderProgram.programObject,"textureSampler");
      int locFlag = glGetUniformLocation(shaderProgram.programObject,"useTexture");
      //if there is some problem
      if (locMV<0 || locN<0 || locP<0 || texLoc <0 || locFlag<0)
      {
          //not all uniforms were allocated - show blue screen.
          //check your variables properly. May be there is unused?
          glClearColor(0,0,1,1);
          glClear(GL_COLOR_BUFFER_BIT);
          //end frame visualization
          glutSwapBuffers();
          return;
      }

      //camera matrix. camera is placed in point "eye" and looks at point "cen".
      glm::mat4x4 viewMatrix = glm::lookAt(eye,cen,up);
      object *p = root;
      std::stack<object *> que;
      que.push(p);
      while (!que.empty())
      {
          p = que.top();
          que.pop();
          std::vector<object *> res = p->getChilds();
          for (unsigned i = 0; i < res.size(); i++) {
              que.push(res[i]);

          }
          //p->init_matrix();
          glm::mat4 modelMatrix = p->getModelM();
          //modelViewMatrix consists of viewMatrix and modelMatrix
          glm::mat4 modelViewMatrix = viewMatrix*modelMatrix;
          //calculate normal matrix
          glm::mat4 normalMatrix = glm::inverseTranspose(modelViewMatrix);
          //finally calculate modelViewProjectionMatrix
          glm::mat4 modelViewProjectionMatrix = proj_matr *modelViewMatrix;
          glUniformMatrix4fv(locMV,1,0,glm::value_ptr(modelViewMatrix));
          glUniformMatrix4fv(locN,1,0,glm::value_ptr(normalMatrix));
          glUniformMatrix4fv(locP,1,0,glm::value_ptr(modelViewProjectionMatrix));
          if (p->getType() == O_BRANCH) {
              //bind texture
              glBindTexture(GL_TEXTURE_2D,texId[0]);

              //pass variables to the shaders

              glUniform1ui(texLoc,0);
              glUniform1i(locFlag,useTexture);
              VBranch->visualize();
          } else {
              VLeaf->visualize();
          }
      }



      //end frame visualization
      //glutSwapBuffers();
}


void Tree::update() {

    if (m_tick >= MAX_TICKS) {
        fprintf(stderr, "tick %d Growing\n", m_tick);
        draw_scene();
        glutSwapBuffers();
        return;
    }
    if (!root) {
        root = new branch(NULL);
    } else {
        if (m_tick > 0 && m_tick % T_GROW == 0) {
            fprintf(stderr, "tick %d Growing\n", m_tick);
            root->grow();
       }
    }
//    root = new branch();
//    m_tick = 5;
//    root->grow();
//    //leaf *a = new leaf();
//    //a->draw();
    draw_scene();
    glutSwapBuffers();

    //return;
    m_tick++;
}




void Tree::gui_init()
{

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    //initialize shader program
    shaderProgram.init(VertexShaderName.c_str(),FragmentShaderName.c_str());
    //use this shader program
    glUseProgram(shaderProgram.programObject);
}

void Tree::gui_pre_frame(glm::mat4 ModelMatrix, int texture_num) {
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    //Draw triangle with shaders (in screen coordinates)
    //need to set uniform in modelViewMatrix

    glUseProgram(shaderProgram.programObject);

    //we will need this uniform locations to connect them to our variables
    int locMV = glGetUniformLocation(shaderProgram.programObject,"modelViewMatrix");
    int locN = glGetUniformLocation(shaderProgram.programObject,"normalMatrix");
    int locP = glGetUniformLocation(shaderProgram.programObject,"modelViewProjectionMatrix");
    int texLoc = glGetUniformLocation(shaderProgram.programObject,"textureSampler");
    int locFlag = glGetUniformLocation(shaderProgram.programObject,"useTexture");
    //if there is some problem

    if (locMV<0 || locN<0 || locP<0 || texLoc <0 || locFlag<0)
    {
        //not all uniforms were allocated - show blue screen.
        //check your variables properly. May be there is unused?
        glClearColor(0,0,1,1);
        glClear(GL_COLOR_BUFFER_BIT);
        //end frame visualization
        glutSwapBuffers();
        throw "BAYDA";
        return;
    }
    view_matr = glm::lookAt(eye,cen,up);
    glm::mat4 mv = view_matr * ModelMatrix;
    glm::mat4 norm = glm::inverseTranspose(mv);

    glm::mat4 mvp = proj_matr * mv;
//    cerr << "MODELM\n" << glm::to_string(ModelMatrix) << endl;
//    cerr << "VM\n" << glm::to_string(view_matr) << endl;
//    cerr << "PM\n" << glm::to_string(proj_matr) << endl;
    if (texture_num != -1) {
        glBindTexture(GL_TEXTURE_2D, texId[texture_num]);
    }


    //pass variables to the shaders
    glUniformMatrix4fv(locMV,1,0,glm::value_ptr(mv));
    glUniformMatrix4fv(locN,1,0,glm::value_ptr(norm));
    glUniformMatrix4fv(locP,1,0,glm::value_ptr(mvp));
    glUniform1ui(texLoc,0);
    glUniform1i(locFlag,useTexture);
    //glutSwapBuffers();
}

void Tree::init_textures()
{
    //generate as many textures as you need
    glGenTextures(1,&texId[0]);

    //enable texturing and zero slot
    glActiveTexture(GL_TEXTURE0);
    //bind texId to 0 unit
    glBindTexture(GL_TEXTURE_2D,texId[0]);

    //don't use alignment
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);

    // Set nearest filtering mode for texture minification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    //TODO: load texture from file
    GLubyte imgData[2*2*3] = {
        //row1: yellow,orange
        255,255,0, 255,128,0,
        //row2: green, dark green
        0,255,0, 0,64,0
    };

    //set Texture Data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2,2, 0, GL_RGB, GL_UNSIGNED_BYTE, &imgData[0]);
}


void Tree::reshape(int width, int height)
{
    windowWidth = width;
    windowHeight = height;
    //set viewport to match window size
    glViewport(0, 0, width, height);

    float fieldOfView = 45.0f;
    float aspectRatio = float(width)/float(height);
    float zNear = 0.1f;
    float zFar = 100.0f;
    //set projection matrix
    proj_matr = glm::perspective(fieldOfView,aspectRatio,zNear,zFar);
}

Tree::~Tree()
{
    delete root;
    glDeleteTextures(1,texId);
}



void Tree::keyboard(unsigned char key, int mx, int my)
{
    static float r = 10;
    const float eps = 0.01;
    const float step = 0.01;

    const float start_angle_fi = M_PI / 2;
    const float start_angle_psi = M_PI / 2;
    static float angle_fi = 0.0;
    static float angle_psi = 0.0;
    /*if (key==' ')
        useTexture = !useTexture;*/
    if (key >= 'A' && key <= 'Z') {
        key += 'a' - 'A';
    }
    switch (key) {
    case 'w':
        angle_psi -= step * M_PI / 2.0f;
        break;
    case 's':
        angle_psi += step * M_PI / 2.0f;
        break;
    case 'a':
        angle_fi += M_PI * step;
        break;
    case 'd':
        angle_fi -= M_PI * step;
        break;
    }
    /*if (angle_psi > M_PI/ 2 - eps || angle_psi < -M_PI / 2 + eps) {
        angle_psi  *= -1.0f;
    }*/


    if (std::fabs(std::fabs(angle_psi) - M_PI / 2.0f) < 0.01   ) {
        angle_psi = (angle_psi > 0)  ? -M_PI / 2.0f + 2.0f * eps : M_PI / 2.0f - 2.0f *eps;
    }

    float res_fi = start_angle_fi + angle_fi;
    float res_psi = start_angle_psi + angle_psi;
    if (fabs(fabs(res_psi) - M_PI) < eps) {
        res_psi *= -1;
    }



    //x =  rco
    eye[0] = r * fsin(res_psi) * fcos(res_fi);
    eye[2] = r * fsin(res_psi) * fsin(res_fi);
    eye[1] = r * fcos(res_psi);
    std::cerr <<"Vector \n" << eye[0] << " " << eye[1] << " " << eye[2] << std::endl;
    std::cerr << "Angles " << angle_fi << " " <<angle_psi  << std::endl;
    //this->draw_scene();
}
