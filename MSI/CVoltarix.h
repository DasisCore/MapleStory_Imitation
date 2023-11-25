#pragma once
#include "CPlayer.h"
class CVoltarix :
    public CPlayer
{



public:
    virtual void update_move() override;
    virtual CVoltarix* Clone() override { return new CVoltarix(*this); }


// skill
public:
    void skill_teleport();


public:
    CVoltarix();
    ~CVoltarix();
};

