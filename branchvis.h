#ifndef BRANCHVIS_H
#define BRANCHVIS_H
#include "objectvis.h"
class branchVis : public objectVis
{
public:
    branchVis();

    virtual ~branchVis(void);
    //redefinition of
    virtual void build_model() override;
};

#endif // BRANCHVIS_H
