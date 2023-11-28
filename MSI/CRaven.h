#pragma once
#include "CPlayer.h"
class CRaven :
    public CPlayer
{


public:
    virtual void update_move() override;
    virtual CRaven* Clone() override { return new CRaven(*this); }

public:
    CRaven();
    ~CRaven();
};

