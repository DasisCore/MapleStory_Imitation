#pragma once
#include "CObject.h"

class CMonster :
    public CObject
{
private:
    float m_fHP;            // ü��
    float m_fSpeed;         // �ӵ�
    float m_fRecogRange;    // ���� ����
    float m_fAttRange;      // ���� ��Ÿ�
    float m_fAtt;           // ���ݷ�

public:
    virtual void update() override;
    CMonster* Clone() { return new CMonster(*this); }

public:
    CMonster();
    ~CMonster();
};

