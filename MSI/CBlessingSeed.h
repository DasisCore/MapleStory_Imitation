#pragma once
#include "CObject.h"

class CBlessingSeed :
    public CObject
{
private:
    CObject* m_pOwner;

    float m_fCastTime;

private:
    // ��ų�� �����Ǵ� �ð� ���
    void CalCastingTime();

public:
    virtual void update();
    virtual void OnCollisionEnter(CCollider* _pOther);
    virtual void OnCollision(CCollider* _pOther);
    virtual CBlessingSeed* Clone() override { return new CBlessingSeed(*this); }

public:
    CBlessingSeed(CObject* _pObj);
    ~CBlessingSeed();
};

