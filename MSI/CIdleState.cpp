#include "global.h"
#include "CIdleState.h"

#include "CSceneMgr.h"
#include "CScene.h"

#include "CPlayer.h"
#include "CMonster.h"
#include "CEventMgr.h"

CIdleState::CIdleState()
	: CState(MON_STATE::IDLE)
{
}
CIdleState::~CIdleState()
{
}

void CIdleState::Enter()
{
}

void CIdleState::Exit()
{
}

void CIdleState::update()
{
	// Player의 위치 체크
	CPlayer* pPlayer = (CPlayer*)CSceneMgr::GetInst()->GetCurScene()->GetPlayer();
	Vec2 vPlayerPos = pPlayer->GetPos();

	// 몬스터의 범위 안에 들어오면 추적상태로 변환
	CMonster* pMonster = GetMonster();
	Vec2 vMonPos = pMonster->GetPos();

	Vec2 vDiff = (vPlayerPos - vMonPos);
	float fLen = vDiff.Length();

	// 플레이어가 몬스터의 인식 범위 안으로 진입
	if (fLen < pMonster->GetInfo().fRecogRange)
	{
		ChangeAIState(GetAI(), MON_STATE::TRACE);
	}
}
