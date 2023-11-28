#pragma once
#include "CMonster.h"
class CLesh :
    public CMonster
{


public:
    virtual void update() override;


public:
    CLesh();
    ~CLesh();
};

