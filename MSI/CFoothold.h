#pragma once

#include "CObject.h"

class CFoothold :
    public CObject
{
private:
    // 발판의 방향을 설정 / 좌 상 우
    bool m_bLeft;
    bool m_bTop;
    bool m_bRight;
    bool m_bBottom;

private:
    virtual void start() override;
    virtual void update() override;
    virtual void render(HDC _dc) override;

public:
    virtual void OnCollisionEnter(CCollider* _pOther) override;
    virtual void OnCollision(CCollider* _pOther) override;
    virtual void OnCollisionExit(CCollider* _pOther) override;
    virtual CFoothold* Clone() override { return new CFoothold(*this); }

private:
    void Blocking(CCollider* _pOther);

public:
    CFoothold();
    CFoothold(wstring _strName, Vec2 _vPos, Vec2 _vScale
        , bool _bCollider, Vec2 _vColOffset, Vec2 _vColScale
        , bool _bAnimation, vector<wstring> _vecPath
        , bool _bGravity, bool _bRigidBody);
    ~CFoothold();
};

