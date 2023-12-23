#pragma once
#include "CObject.h"

class CNatureGrab :
    public CObject
{
private:
    CObject* m_pOwner;
    int m_iDir;
    float m_fCastTime;

private:
    // ��ų�� �����Ǵ� �ð� ���
    void CalCastingTime();

public:
    virtual void update();
    virtual void OnCollisionEnter(CCollider* _pOther);
    virtual void OnCollision(CCollider* _pOther);
    virtual CNatureGrab* Clone() override { return new CNatureGrab(*this); }

public:
    CNatureGrab(CObject* _pObj);
    ~CNatureGrab();
};

