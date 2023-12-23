#pragma once
#include "CObject.h"

struct tMonInfo
{
    float fHP;              // 체력
    float fSpeed;           // 속도
    Vec2 vRecogRange;       // 인지 범위
    float fRecogRange;       // 추적 범위
    float fAttRange;        // 공격 사거리
    float fAtt;             // 공격력
    int iDir;               // 방향
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
    // 편의상 Vec2로 리턴해줌. 몬스터가 접촉한 Ground의 남은 거리 (왼쪽, 오른쪽)
    Vec2 GetRemainDist() { return Vec2(m_fLeftDist, m_fRightDist); }
    void SetDead() { m_bDead = true; }

public:
    void update_animation();


public:
    virtual void update() override;
    virtual void render(HDC _dc) override;
    CMonster* Clone() { return new CMonster(*this); }

    
//private:  // 원래 private여야 함. 수정하지 못해야 함. Lesh등을 위해 잠시 풀어둠. 수정해야 함.
// 몬스터 각각의 생성자마다. 자신의 tMonInfo를 가지도록.
public:
    void SetMonInfo(const tMonInfo& _info) { m_tInfo = _info; }

public:
    CMonster();
    ~CMonster();

    friend class CMonFactory;
};

