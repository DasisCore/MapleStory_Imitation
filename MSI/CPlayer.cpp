#include "global.h"
#include "CPlayer.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"

CPlayer::CPlayer()
	: m_eCurState(PLAYER_STATE::IDLE)
	, m_ePrevState(PLAYER_STATE::WALK)
	, m_iDir(0)
	, m_iPrevDir(0)
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::update()
{
	Vec2 vPos = GetPos();

	if (KEY_HOLD(KEY::W))
	{
		vPos.y -= 100.f * DT;
		SetPos(vPos);
	}

	if (KEY_HOLD(KEY::A))
	{
		vPos.x -= 100.f * DT;
		SetPos(vPos);
	}

	if (KEY_HOLD(KEY::S))
	{
		vPos.y += 100.f * DT;
		SetPos(vPos);
	}

	if (KEY_HOLD(KEY::D))
	{
		vPos.x += 100.f * DT;
		SetPos(vPos);
	}
}

//void CPlayer::render(HDC _dc)
//{
//}
