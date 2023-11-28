#pragma once
#include "CState.h"

class CCollider;

class CAttackState :
    public CState
{
private:
    CCollider* m_pCollider;


public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void update() override;


public:
    CAttackState();
    ~CAttackState();
};

