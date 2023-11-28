#pragma once
#include "CObject.h"

struct tMonInfo
{
    float fHP;            // 체력
    float fSpeed;         // 속도
    float fRecogRange;    // 인지 범위
    float fAttRange;      // 공격 사거리
    float fAtt;           // 공격력
    int mDir;             // 방향
};

class AI;

class CMonster :
    public CObject
{
private:
    wstring m_wCurChar;
    tMonInfo m_tInfo;
    AI* m_pAI;


public:
    float GetSpeed() { return m_tInfo.fSpeed; }
    void SetSpeed(float _fSpeed) { m_tInfo.fSpeed = _fSpeed; }
    void setCurChar(wstring _wstrChar) { m_wCurChar = _wstrChar; }

    void SetAI(AI* _pAI);
    const tMonInfo& GetInfo() { return m_tInfo; }
    void SetRandomDir();

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

