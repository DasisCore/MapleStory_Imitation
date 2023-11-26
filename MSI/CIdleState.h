#pragma once
#include "CState.h"
class CIdleState :
    public CState
{
private:
    float m_fWaitTime;


public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void update() override;


public:
    CIdleState();
    ~CIdleState();
};

