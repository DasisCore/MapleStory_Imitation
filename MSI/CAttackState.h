#pragma once
#include "CState.h"

class CDetect;

class CAttackState :
    public CState
{
private:
    CDetect* m_pDetect;
    
    // ������ �������� ��������� ������.
    // �� m_fAttack�� 0.5���, Detect���� ���� ��, 0.5�� �ڿ��� �������̶�� 
    // �������� ������.
    float m_fAttack;

    // �̹� ���¿��� ������ �ߴ���.
    bool m_bAttack;

public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void update() override;


public:
    CAttackState();
    ~CAttackState();
};

