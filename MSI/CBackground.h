#pragma once
#include "CObject.h"
class CBackground :
    public CObject
{
private:
    virtual void update() override;

public:
    virtual CBackground* Clone() override { return new CBackground(*this); }

public:
    CBackground();
    CBackground(wstring _strName, Vec2 _vPos, Vec2 _vScale
        , bool _bCollider, Vec2 _vColOffset, Vec2 _vColScale
        , bool _bAnimation, vector<wstring> _vecPath
        , bool _bGravity, bool _bRigidBody);
    ~CBackground();
    
};

