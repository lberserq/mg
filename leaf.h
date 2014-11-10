#ifndef LEAF_H
#define LEAF_H
#include "object.h"
class leaf : public object
{
public:
    leaf(object *parent = NULL):object(O_LEAF, parent) {}
    //void visualize();
    void build_model() override;
    ~leaf() {

    }
};

#endif // LEAF_H
