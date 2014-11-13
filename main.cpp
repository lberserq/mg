//standard libraries
#include <iostream>
#include <unistd.h>
using namespace std;

//opengl headers
#include <GL/glew.h>
#include <GL/freeglut.h>

//opengl mathematics
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

//functions for shader compilation and linking
#include "shaderhelper.h"
//object for drawing
#include "branchvis.h"
#include "leafvis.h"
#include "tree.h"







Tree *world;
void display() {
    world->update();
}

////////////////////////////////////////////////////////////////////////////
/////IdleFunction
void update()
{
    //make animation

    glutPostRedisplay();
    usleep(100000);
}





////////////////////////////////////////////////////////////////////////
///this function is used in case of InitializationError
void emptydisplay()
{
}

////////////////////////////////////////////////////////////////////////
///entry point
int main (int argc, char* argv[])
{
    glutInit(&argc, argv);
#ifdef __APPLE__
    glutInitDisplayMode( GLUT_3_2_CORE_PROFILE| GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#else
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitContextVersion (3, 2);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags (GLUT_FORWARD_COMPATIBLE);
    glewExperimental = GL_TRUE;
#endif

    glutCreateWindow("Test OpenGL application");
    glutDisplayFunc(display);
    glutReshapeFunc(Tree::reshape);
    glutReshapeWindow(800,600);
    glutIdleFunc(update);
    glutKeyboardFunc(Tree::keyboard);
    //glutMouseFunc(mouse);

    glewInit();
    const char * slVer = (const char *) glGetString ( GL_SHADING_LANGUAGE_VERSION );
    cout << "GLSL Version: " << slVer << endl;

    try
    {
        world = new Tree(true);
    }
    catch (const char *str)
    {
        cout << "Error During Initialiation: " << str << endl;
        //	  delete pBranch;
        //  delete pLeaf;
        //glDeleteTextures(1,texId);
        //start main loop with empty screen
        delete world;
        glutDisplayFunc(emptydisplay);
        glutMainLoop();
        return -1;
    }


    try
    {
        glutMainLoop();
    }
    catch (const char *str)
    {
        cout << "Error During Main Loop: " << str << endl;
    }
    //release memory
    //  delete pBranch;
    //  delete pLeaf;

    //  glDeleteTextures(1,texId);
    return 0;
}
