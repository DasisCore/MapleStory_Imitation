#pragma once
#include "CObject.h"

class CGround :
    public CObject
{
private:
    virtual void start() override;
    virtual void update() override;

public:
    virtual void OnCollisionEnter(CCollider* _pOther) override;
    virtual void OnCollision(CCollider* _pOther) override;
    virtual void OnCollisionExit(CCollider* _pOther) override;
    virtual CGround* Clone() override { return new CGround(*this); }

public:
    CGround();
    ~CGround();
};

