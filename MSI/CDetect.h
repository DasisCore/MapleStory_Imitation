#pragma once
#include "CObject.h"
class CDetect :
    public CObject
{
private:
    bool m_bIsDetect;


public:
    bool IsDetect() { return m_bIsDetect; }

public:
    virtual void update() override;
    CDetect* Clone() { return new CDetect(*this); }

public:
    virtual void OnCollisionEnter(CCollider* _pOther);


public:
    CDetect();
    ~CDetect();
};

