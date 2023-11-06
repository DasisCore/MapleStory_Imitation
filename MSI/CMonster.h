#pragma once
#include "CObject.h"

struct tMonInfo
{
    float fHP;            // ü��
    float fSpeed;         // �ӵ�
    float fRecogRange;    // ���� ����
    float fAttRange;      // ���� ��Ÿ�
    float fAtt;           // ���ݷ�
};


class AI;

class CMonster :
    public CObject
{
private:
    tMonInfo m_tInfo;
    AI* m_pAI;

public:
    float GetSpeed() { return m_tInfo.fSpeed; }
    void SetSpeed(float _fSpeed) { m_tInfo.fSpeed = _fSpeed; }
    void SetAI(AI* _pAI);
    const tMonInfo& GetInfo() { return m_tInfo; }


public:
    virtual void update() override;
    CMonster* Clone() { return new CMonster(*this); }

private:
    void SetMonInfo(const tMonInfo& _info) { m_tInfo = _info; }

public:
    CMonster();
    ~CMonster();

    friend class CMonFactory;
};

