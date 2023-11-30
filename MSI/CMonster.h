#pragma once
#include "CObject.h"

struct tMonInfo
{
    float fHP;            // ü��
    float fSpeed;         // �ӵ�
    Vec2 vRecogRange;    // ���� ����
    float fAttRange;      // ���� ��Ÿ�
    float fAtt;           // ���ݷ�
    int iDir;             // ����
};

class AI;

class CMonster :
    public CObject
{
private:
    wstring m_wCurChar;
    tMonInfo m_tInfo;
    AI* m_pAI;

    float m_fLeftDist;
    float m_fRightDist;


public:
    float GetSpeed() { return m_tInfo.fSpeed; }
    void SetSpeed(float _fSpeed) { m_tInfo.fSpeed = _fSpeed; }
    void setCurChar(wstring _wstrChar) { m_wCurChar = _wstrChar; }

    void SetAI(AI* _pAI);
    const tMonInfo& GetInfo() { return m_tInfo; }
    void SetRandomDir();
    void SetMonDir(int _iDir) { m_tInfo.iDir = _iDir; }

    void SetRemainDist(float _fLeft, float _fRight) { m_fLeftDist = _fLeft; m_fRightDist = _fRight; }
    // ���ǻ� Vec2�� ��������.
    Vec2 GetRemainDist() { return Vec2(m_fLeftDist, m_fRightDist); }

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

