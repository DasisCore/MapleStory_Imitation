#pragma once
#include "CState.h"
class CHitState :
    public CState
{
private:
    // 맞은 상태 지속 시간
    float m_fHitTime;


public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void update() override;

public:
    CHitState();
    ~CHitState();
};
