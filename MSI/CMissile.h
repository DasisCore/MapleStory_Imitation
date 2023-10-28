#pragma once
#include "CObject.h"
class CMissile :
    public CObject
{
private:


public:
    virtual void update() override;


public:
    CMissile();
    ~CMissile();
};

