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
    CGround(wstring _strName, Vec2 _vPos, Vec2 _vScale
        , bool _bCollider, Vec2 _vColOffset, Vec2 _vColScale
        , bool _bAnimation, vector<wstring> _vecPath
        , bool _bGravity, bool _bRigidBody);
    ~CGround();
};

