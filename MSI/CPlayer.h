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


struct tPlayerInfo
{
    int iLevel;
    int iMaxHP;
    int iHP;
    int iMaxMP;
    int iMP;
    int iMaxExp;
    int iExp;
};

class CTexture;

class CPlayer :
    public CObject
{
private:
    wstring m_wCurChar;
    tPlayerInfo m_tPlayerInfo;

    PLAYER_STATE m_eCurState;
    PLAYER_STATE m_ePrevState;

    int m_iDir;
    int m_iPrevDir;

    bool m_bIsGround;
    bool m_bIsAir;
    bool m_bUseDoubleJump;
    bool m_bCanOtherAction;

    float m_fDelayTime;
    bool m_bDelay;

    // ���� �ð�
    float m_fUnbeatableTime;

    // ������ ���ڰŸ� �ð� ���
    float m_fBlinkTime;

public:
    virtual void update() override;
    //virtual void render(HDC _dc) override;

public:
    int GetDir() { return m_iDir; }
    void update_state();
    virtual void update_move();
    void update_animation();

    // ĳ���� ���� ������Ʈ (HP, MP, EXP...)
    void update_status();

    virtual void OnCollisionEnter(CCollider* _pOther) override;
    virtual void OnCollision(CCollider* _pOther) override;

    bool GetIsGround() { return m_bIsGround; }
    void SetIsGround(bool _b) { m_bIsGround = _b; }
    bool GetIsAir() { return m_bIsAir; }
    void SetIsAir(bool _b) { m_bIsAir = _b; }

    // ĳ������ ���� ��ȯ
    void SetCurState(PLAYER_STATE _eState) { m_eCurState = _eState; }
    wstring GetCurChar() { return m_wCurChar; }

    // ĳ���� ���� ��������
    const tPlayerInfo GetPlayerInfo() { return m_tPlayerInfo; }

    // ü�� ������.
    void AddPlayerHP(int _i) { m_tPlayerInfo.iHP += _i; }

    // ĳ���� ������ ���
    void Delay();
    void SetDelay(float _fDelay) { m_fDelayTime = _fDelay; }
    bool GetCanOtherAction() { return m_bCanOtherAction; }
    virtual CPlayer* Clone() override { return new CPlayer(*this); }
    
    // ���� ĳ���Ͱ� �������� �ƴ���
    bool IsUnbeatable() { return m_fUnbeatableTime >= 0; }

// �ӽ�
public:
    virtual void render(HDC _dc);
    void show_state(HDC _dc);

// ���� �÷��̿� ���õ� ��� �Լ�
public:
    void CharHit(int _iDir, int _iDamege);
    void AddExp(int _iExp) { m_tPlayerInfo.iExp += _iExp; }


public:
    CPlayer();
    ~CPlayer();
};

