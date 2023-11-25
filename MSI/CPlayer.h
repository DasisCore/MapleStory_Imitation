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
    END,
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

public:
    void setCurChar(wstring _wstrChar) { m_wCurChar = _wstrChar; }

public:
    virtual void update() override;
    //virtual void render(HDC _dc) override;

public:
    void CreateMissile();
    void update_state();
    virtual void update_move();
    void update_animation();

    virtual void OnCollisionEnter(CCollider* _pOther) override;

    virtual CPlayer* Clone() override { return new CPlayer(*this); }

// юс╫ц
public:
    virtual void render(HDC _dc);


public:
    CPlayer();
    ~CPlayer();
};

