#include "global.h"
#include "CTraceState.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CPlayer.h"
#include "CMonster.h"

#include "CTimeMgr.h"

CTraceState::CTraceState()
	: CState(MON_STATE::TRACE)
{
}

CTraceState::~CTraceState()
{
}


void CTraceState::Enter()
{
}

void CTraceState::Exit()
{
}

void CTraceState::update()
{
	// Å¸°Ù ÃßÀû
	CPlayer* pPlayer = (CPlayer*)CSceneMgr::GetInst()->GetCurScene()->GetPlayer();
	Vec2 vPlayerPos = pPlayer->GetPos();

	Vec2 vMonPos = GetMonster()->GetPos();

	Vec2 vMonDir = vPlayerPos - vMonPos;
	if (vMonDir.IsZero()) return;

	vMonDir.Normalize();

	vMonPos += vMonDir * GetMonster()->GetInfo().fSpeed * fDT;

	GetMonster()->SetPos(vMonPos);
}
