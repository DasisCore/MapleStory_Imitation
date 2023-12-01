#pragma once
#include "CState.h"

class CDetect;

class CTraceState :
    public CState
{
private:
    CDetect* m_pDetect;
    
    // 플레이어를 n초 단위로 추적한다.
    float m_fTraceTime;
    // 플레이어가 인식 범위를 벗어나고 n초간 플레이어를 더 추적함
    float m_fRemainTraceTime;  
    // 몬스터의 방향
    int m_iDir;


public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void update() override;

public:
    CTraceState();
    ~CTraceState();
};

