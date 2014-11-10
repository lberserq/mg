#pragma once
#include "object.h"

//branch of tree is cylinder
class branch :
    public object
{
public:
    branch(object *parent = NULL);
    virtual ~branch(void);
	//redefinition of 
    virtual void build_model() override;
};

