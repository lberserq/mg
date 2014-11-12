#ifndef LEAF_H
#define LEAF_H
#include "object.h"
class leaf : public object
{
    void init_matrix() override;
public:
    leaf(object *parent = NULL):object(O_LEAF, parent)
    {

    }
    //void visualize();
    virtual void grow() override {
        ; //not realised in the list
    }

    void draw() override;
};

#endif // LEAF_H
