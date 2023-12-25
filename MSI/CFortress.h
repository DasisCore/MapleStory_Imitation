#pragma once
#include "CObject.h"
class CFortress :
    public CObject
{
private:
    CObject* m_pOwner;

    float m_fCastBegin;
    float m_fCastRepeat;
    float m_fCastEnd;

private:
    // 스킬이 시전되는 시간 계산
    void CalCastingTime();

public:
    virtual void update();
    virtual void OnCollisionEnter(CCollider* _pOther);
    virtual void OnCollision(CCollider* _pOther);
    virtual CFortress* Clone() override { return new CFortress(*this); }

public:
    CFortress(CObject* _pObj);
    ~CFortress();
};

