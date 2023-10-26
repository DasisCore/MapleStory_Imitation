#pragma once
#include "CObject.h"

class CMonster :
    public CObject
{



public:
    virtual void update() override;

public:
    CMonster();
    ~CMonster();
};

