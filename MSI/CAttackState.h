#pragma once
#include "CState.h"

class CDetect;

class CAttackState :
    public CState
{
private:
    CDetect* m_pDetect;
    
    // 실제로 데미지가 들어가기까지의 딜레이.
    // 즉 m_fAttack이 0.5라면, Detect에서 감지 후, 0.5초 뒤에도 감지중이라면 
    // 데미지를 입힌다.
    float m_fAttack;

    // 이번 상태에서 공격을 했는지.
    bool m_bAttack;

public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void update() override;


public:
    CAttackState();
    ~CAttackState();
};

