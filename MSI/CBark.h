#pragma once
#include "CPlayer.h"
class CBark :
    public CPlayer
{

public:
    virtual void update() override;
    //virtual void update_move() override;
    virtual CBark* Clone() override { return new CBark(*this); }


    // skill
public:
    //void skill_teleport();

public:
    CBark();
    ~CBark();
};

