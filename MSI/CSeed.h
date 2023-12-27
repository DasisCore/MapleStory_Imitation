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

private:
    // ��ų�� �����Ǵ� �ð� ���
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

