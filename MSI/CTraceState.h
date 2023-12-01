#pragma once
#include "CState.h"

class CDetect;

class CTraceState :
    public CState
{
private:
    CDetect* m_pDetect;
    
    // �÷��̾ n�� ������ �����Ѵ�.
    float m_fTraceTime;
    // �÷��̾ �ν� ������ ����� n�ʰ� �÷��̾ �� ������
    float m_fRemainTraceTime;  
    // ������ ����
    int m_iDir;


public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void update() override;

public:
    CTraceState();
    ~CTraceState();
};

