#pragma once
#include "CObject.h"

enum class PLAYER_STATE
{
    IDLE,
    WALK,
    ALERT,
    ATTACK,
    JUMP,
    DEAD,
    PRONE,
    PRONE_ATT,
    END = 20,
};

enum class PLAYER_ATTACK_STATE
{
    NORMAL_ATT_1,
    NORMAL_ATT_2,
    NORMAL_ATT_3,
    SKILL_ATT_1,
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CTexture;

class CPlayer :
    public CObject
{
private:
    wstring m_wCurChar;

    PLAYER_STATE m_eCurState;
    PLAYER_STATE m_ePrevState;

    int m_iDir;
    int m_iPrevDir;

    bool m_bIsGround;
    bool m_bIsAir;
    bool m_bCanOtherAction;

    float m_fDelayTime;
    bool m_bDelay;

public:
    virtual void update() override;
    //virtual void render(HDC _dc) override;

public:
    int GetDir() { return m_iDir; }
    void CreateMissile();
    void update_state();
    virtual void update_move();
    void update_animation();

    virtual void OnCollisionEnter(CCollider* _pOther) override;
    
    bool GetIsGround() { return m_bIsGround; }
    void SetIsGround(bool _b) { m_bIsGround = _b; }
    bool GetIsAir() { return m_bIsAir; }
    void SetIsAir(bool _b) { m_bIsAir = _b; }

    // ĳ������ ���� ��ȯ
    void SetCurState(PLAYER_STATE _eState) { m_eCurState = _eState; }

    // ĳ���� ������ ���
    void Delay();
    void SetDelay(float _fDelay) { m_fDelayTime = _fDelay; }
    bool GetCanOtherAction() { return m_bCanOtherAction; }
    virtual CPlayer* Clone() override { return new CPlayer(*this); }

// �ӽ�
public:
    virtual void render(HDC _dc);
    void show_state(HDC _dc);

// ���� �÷��̿� ���õ� ��� �Լ�
public:
    void CharHit(int _iDir);


public:
    CPlayer();
    ~CPlayer();
};

