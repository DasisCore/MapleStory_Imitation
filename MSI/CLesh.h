#pragma once
#include "CMonster.h"
class CLesh :
    public CMonster
{
private:
    // �����ϴµ� ��� �ð�
    float m_fAttDelay;
    float m_fAttTime;

public:
    void Attack();
    void SetAttTime() { m_fAttTime = 1.5f; }
    bool IsFinshAtt()
    {
        if (m_fAttTime < 0)
        {
            m_fAttTime = m_fAttDelay;
            return true;
        }
        return false;
    }

public:
    virtual void update() override;

public:
    CLesh();
    ~CLesh();
};

