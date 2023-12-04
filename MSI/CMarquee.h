#pragma once
#include "CObject.h"
class CMarquee :
    public CObject
{



public:
    virtual void update() override;
    virtual void render(HDC _dc) override;
    CMarquee* Clone() { return new CMarquee(*this); }

public:
    CMarquee();
    ~CMarquee();
};

