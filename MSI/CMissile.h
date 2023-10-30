#pragma once
#include "CObject.h"
class CMissile :
    public CObject
{
private:


public:
    virtual void update() override;
    CMissile* Clone() { return new CMissile(*this); }

public:
    CMissile();
    ~CMissile();
};

