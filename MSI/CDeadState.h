#pragma once
#include "CState.h"
class CDeadState :
    public CState
{
private:
    float m_DeadDelay;



public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void update() override;

public:
    CDeadState();
    ~CDeadState();
};

