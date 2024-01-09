#pragma once
#include "CObject.h"
class CSeed :
    public CObject
{
private:
    CObject* m_pOwner;

    float m_fCastBegin;
    float m_fCastRepeat;
    float m_fCastEnd;

//public:
    // 모든 Seed가 공유하는 힐 쿨타임.
    static float m_fHealDelay;

private:
    // 스킬이 시전되는 시간 계산
    void CalCastingTime();

public:
    virtual void update();
    virtual void OnCollisionEnter(CCollider* _pOther);
    virtual void OnCollision(CCollider* _pOther);
    virtual CSeed* Clone() override { return new CSeed(*this); }

public:
    CSeed(CObject* _pObj);
    ~CSeed();

};

