#pragma once
#include "CMonster.h"

class CMonster_Normal :
    public CMonster
{
private:
    // 공격하는데 드는 시간
    float m_fAttDelay;
    float m_fAttTime;

public:
    void Attack();
    void SetAttTime() { m_fAttTime = 1.5f; }
    bool IsFinshAtt()
    {
        if (m_fAttTime < 0)
        {
            m_fAttTime = m_fAttDelay;
            return true;
        }
        return false;
    }

public:
    virtual void update() override;

public:
    CMonster_Normal();
    CMonster_Normal(wstring _strName, Vec2 _vPos, Vec2 _vScale
        , bool _bCollider, Vec2 _vColOffset, Vec2 _vColScale
        , bool _bAnimation, vector<wstring> _vecPath
        , bool _bGravity, bool _bRigidBody);
    ~CMonster_Normal();
};

