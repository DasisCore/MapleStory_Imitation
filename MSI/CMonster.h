#pragma once
#include "CObject.h"

class CMonster :
    public CObject
{
private:
    float m_fHP;            // 체력
    float m_fSpeed;         // 속도
    float m_fRecogRange;    // 인지 범위
    float m_fAttRange;      // 공격 사거리
    float m_fAtt;           // 공격력

public:
    virtual void update() override;
    CMonster* Clone() { return new CMonster(*this); }

public:
    CMonster();
    ~CMonster();
};

