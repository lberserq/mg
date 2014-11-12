#ifndef OBJECTVIS_H
#define OBJECTVIS_H
#include <glm/glm.hpp>
struct VertexData{
    glm::vec3 pos;
    glm::vec3 nor;
    glm::vec2 tex;
};

class objectVis
{
protected:
    VertexData* pData;	//pointer to object's internal data
    unsigned int dataCount;

    unsigned int* pIndices;	//pointer to indexes (list of vetrices)
    unsigned int indicesCount;

    unsigned int vbo[2];//VertexBufferObject one for MeshVertexData, another for Indexes
    unsigned int vao;//one VertexArrayObject
public:
    objectVis():
        dataCount(0),
        pIndices(0),
        indicesCount(0),
        pData(0)
    {

    }
    virtual void visualize();
    virtual void build_model() = 0;
    void initGLBuffers(unsigned programId, const char* posName, const char* norName, const char* texName);
    virtual ~objectVis();

};

#endif // OBJECTVIS_H
