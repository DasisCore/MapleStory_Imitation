#pragma once
#include "CState.h"

class CPatrolState :
    public CState
{
private:
    float m_fMoveTime;


public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void update() override;

public:
    CPatrolState();
    ~CPatrolState();
};
