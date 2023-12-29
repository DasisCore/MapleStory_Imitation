#pragma once
#include "CObject.h"
class CDetect :
    public CObject
{
private:
    bool m_bIsDetect;

    // Detect�� ������ ������Ʈ.
    CObject* m_pObject;

public:
    bool IsDetect() { return m_bIsDetect; }
    CObject* GetDetectObj() { return m_pObject; }

public:
    virtual void update() override;
    CDetect* Clone() { return new CDetect(*this); }

public:
    virtual void OnCollisionEnter(CCollider* _pOther);
    virtual void OnCollisionExit(CCollider* _pOther);


public:
    CDetect();
    ~CDetect();
};

