#pragma once
#include "CState.h"

class CPatrolState :
    public CState
{
private:
    float m_fMoveTime;
    int m_iDir;

private:
    // 기본적인 순찰
    void Patrol();
    // 순찰 중, 탐지 범위 안에 플레이어가 들어왔을 경우
    bool InRangePlayer();

public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void update() override;

public:
    CPatrolState();
    ~CPatrolState();
};

