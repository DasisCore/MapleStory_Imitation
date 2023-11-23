#pragma once
#include "CPlayer.h"
class CVoltarix :
    public CPlayer
{



public:
    virtual CVoltarix* Clone() override { return new CVoltarix(*this); }

public:
    CVoltarix();
    ~CVoltarix();
};

