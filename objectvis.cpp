#include "objectvis.h"
#include <GL/glew.h>
#include <GL/freeglut.h>


objectVis::~objectVis()
{
    glDeleteBuffers(2,vbo);
    glDeleteVertexArrays(1,&vao);
    if (pData)
    {
        delete[] pData;
        delete[] pIndices;
    }
}

void objectVis::initGLBuffers(GLuint programId, const char* posName, const char* norName, const char* texName)
{
    glGenVertexArrays( 1, &vao );
    glBindVertexArray ( vao );

    glGenBuffers ( 2, &vbo[0]);

    glBindBuffer ( GL_ARRAY_BUFFER, vbo[0] );
    glBufferData ( GL_ARRAY_BUFFER, dataCount*sizeof(VertexData), pData, GL_STATIC_DRAW );

    glEnable(GL_ELEMENT_ARRAY_BUFFER);
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, vbo[1] );
    glBufferData ( GL_ELEMENT_ARRAY_BUFFER, indicesCount*sizeof(unsigned int), pIndices, GL_STATIC_DRAW );

    int	loc = glGetAttribLocation(programId, posName);
    if (loc>-1)
    {
        glVertexAttribPointer(loc,3,GL_FLOAT,GL_FALSE,sizeof(VertexData),(GLuint *)0);
        glEnableVertexAttribArray (loc);
    }
    int loc2 = glGetAttribLocation(programId, norName);
    if (loc2>-1)
    {
        glVertexAttribPointer(loc2,3,GL_FLOAT,GL_FALSE,sizeof(VertexData),(GLuint *)(0+sizeof(float)*3));
        glEnableVertexAttribArray (loc2);
    }
    int loc3 = glGetAttribLocation(programId, texName);
    if (loc3>-1)
    {
        glVertexAttribPointer(loc3,2,GL_FLOAT,GL_FALSE,sizeof(VertexData),(GLuint *)(0+sizeof(float)*6));
        glEnableVertexAttribArray (loc3);
    }
    glBindVertexArray(0);

}



void objectVis::visualize()
{
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES,indicesCount,GL_UNSIGNED_INT,0);
    glBindVertexArray(0);
}
