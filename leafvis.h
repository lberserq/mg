#ifndef LEAFVIS_H
#define LEAFVIS_H
#include "objectvis.h"
class leafVis : public objectVis
{
public:
    leafVis();
    void build_model() override;
    ~leafVis() {
    }
};

#endif // LEAFVIS_H
