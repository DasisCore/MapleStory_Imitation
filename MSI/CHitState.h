#pragma once
#include "CState.h"
class CHitState :
    public CState
{
private:
    // ���� ���� ���� �ð�
    float m_fHitTime;


public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void update() override;

public:
    CHitState();
    ~CHitState();
};
