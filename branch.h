#pragma once
#include "object.h"

enum
{
    MAX_DEPTH = 20,
    MAX_CAPACITY = 20
};
//branch of tree is cylinder
class branch : public object
{
    friend class object;
    int level_id;

    void init_matrix() override;
public:
    branch(object *parent = NULL);
    virtual ~branch(void);
    //redefinition of
    void set_level(int id) {
        level_id = id;
    }
    virtual void add_child(object *obj) override;
    void draw() override;
    void grow() override;
};

