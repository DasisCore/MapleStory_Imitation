#pragma once
#include "CState.h"

class CDetect;

class CAttackState :
    public CState
{
private:
    CDetect* m_pDetect;


public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void update() override;


public:
    CAttackState();
    ~CAttackState();
};

