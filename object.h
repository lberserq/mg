#ifndef OBJECT_H
#define OBJECT_H
#include <vector>
#include <glm/glm.hpp>
enum OBJ_TYPE{
    O_LEAF = 1,
    O_BRANCH = 2,
    O_OTHER = 3

};

struct VertexData{
    glm::vec3 pos;
    glm::vec3 nor;
    glm::vec2 tex;
};

class object
{
protected:
    VertexData* pData;	//pointer to object's internal data
    unsigned int dataCount;

    unsigned int* pIndices;	//pointer to indexes (list of vetrices)
    unsigned int indicesCount;

    unsigned int vbo[2];//VertexBufferObject one for MeshVertexData, another for Indexes
    unsigned int vao;//one VertexArrayObject

    OBJ_TYPE m_type;
    std::vector<object *> childs;
    object *m_parent;
    //Matrix my_reprentation;
public:

    object(OBJ_TYPE type, object  *parent = NULL):m_type(type),
        m_parent(parent),
        dataCount(0),
        indicesCount(0),
        pData(0),
        pIndices(0) {
    }
    virtual void add_child(object *child) {
        childs.push_back(child);
    }
    virtual void visualize();
    virtual void build_model() = 0;
    virtual ~object();
    void initGLBuffers(unsigned programId, const char* posName,const char* norName,const char* texName);
};

#endif // OBJECT_H
