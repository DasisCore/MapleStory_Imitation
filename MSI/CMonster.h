#pragma once
#include "CObject.h"

struct tMonInfo
{
    float fHP;              // ü��
    float fSpeed;           // �ӵ�
    Vec2 vRecogRange;       // ���� ����
    float fRecogRange;       // ���� ����
    float fAttRange;        // ���� ��Ÿ�
    float fAtt;             // ���ݷ�
    int iDir;               // ����
};

class AI;

class CMonster :
    public CObject
{
private:
    tMonInfo m_tInfo;
    AI* m_pAI;

    float m_fLeftDist;
    float m_fRightDist;

    bool m_bDead;


public:
    float GetSpeed() { return m_tInfo.fSpeed; }
    void SetSpeed(float _fSpeed) { m_tInfo.fSpeed = _fSpeed; }

    void SetAI(AI* _pAI);
    const tMonInfo& GetInfo() { return m_tInfo; }
    void SetRandomDir();
    void SetMonDir(int _iDir) { m_tInfo.iDir = _iDir; }
    void SetRemainDist(float _fLeft, float _fRight) { m_fLeftDist = _fLeft; m_fRightDist = _fRight; }
    void TakeDamege(float m_fDamege) { m_tInfo.fHP -= m_fDamege; }
    // ���ǻ� Vec2�� ��������. ���Ͱ� ������ Ground�� ���� �Ÿ� (����, ������)
    Vec2 GetRemainDist() { return Vec2(m_fLeftDist, m_fRightDist); }
    void SetDead() { m_bDead = true; }

public:
    void update_animation();


public:
    virtual void update() override;
    virtual void render(HDC _dc) override;
    CMonster* Clone() { return new CMonster(*this); }

    
//private:  // ���� private���� ��. �������� ���ؾ� ��. Lesh���� ���� ��� Ǯ���. �����ؾ� ��.
// ���� ������ �����ڸ���. �ڽ��� tMonInfo�� ��������.
public:
    void SetMonInfo(const tMonInfo& _info) { m_tInfo = _info; }

public:
    CMonster();
    ~CMonster();

    friend class CMonFactory;
};

