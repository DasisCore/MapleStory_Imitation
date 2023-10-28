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
    END
};

enum class PLAYER_ATTACK_STATE
{
    NORMAL_ATT_1,
    NORMAL_ATT_2,
    NORMAL_ATT_3,
    SKILL_ATT_1,
};


class CPlayer :
    public CObject
{
private:
    PLAYER_STATE m_eCurState;
    PLAYER_STATE m_ePrevState;

    int m_iDir;
    int m_iPrevDir;

public:
    virtual void update() override;
    //virtual void render(HDC _dc) override;

public:
    void CreateMissile();



public:
    CPlayer();
    ~CPlayer();
};

