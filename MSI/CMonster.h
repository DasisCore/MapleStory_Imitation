#pragma once
#include "CObject.h"

struct tMonInfo
{
    float fHP;            // 체력
    float fSpeed;         // 속도
    float fRecogRange;    // 인지 범위
    float fAttRange;      // 공격 사거리
    float fAtt;           // 공격력
};


class AI;

class CMonster :
    public CObject
{
private:
    tMonInfo m_tInfo;
    AI* m_pAI;

public:
    float GetSpeed() { return m_tInfo.fSpeed; }
    void SetSpeed(float _fSpeed) { m_tInfo.fSpeed = _fSpeed; }
    void SetAI(AI* _pAI);
    const tMonInfo& GetInfo() { return m_tInfo; }


public:
    virtual void update() override;
    CMonster* Clone() { return new CMonster(*this); }

private:
    void SetMonInfo(const tMonInfo& _info) { m_tInfo = _info; }

public:
    CMonster();
    ~CMonster();

    friend class CMonFactory;
};

