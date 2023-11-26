#include "global.h"
#include "CIdleState.h"

#include "CSceneMgr.h"
#include "CScene.h"

#include "CPlayer.h"
#include "CMonster.h"
#include "CEventMgr.h"

#include "CRandom.h"
#include "CTimeMgr.h"

CIdleState::CIdleState()
	: CState(MON_STATE::IDLE)
	, m_fWaitTime(2.f)
{
}
CIdleState::~CIdleState()
{
}

void CIdleState::Enter()
{
	// 1.5 ~ 3초 랜덤으로 대기하는 시간을 가짐.
	m_fWaitTime = CRandom::GetInst()->GetBetweenReal(1.f, 2.f);
	GetMonster()->SetRandomDir();
}

void CIdleState::Exit()
{
}

void CIdleState::update()
{
	m_fWaitTime -= fDT;

	// Player의 위치 체크
	CPlayer* pPlayer = (CPlayer*)CSceneMgr::GetInst()->GetCurScene()->GetPlayer();
	Vec2 vPlayerPos = pPlayer->GetPos();

	// 몬스터의 범위 안에 들어오면 추적상태로 변환
	CMonster* pMonster = GetMonster();
	Vec2 vMonPos = pMonster->GetPos();

	Vec2 vDiff = (vPlayerPos - vMonPos);
	float fLen = vDiff.Length();

	// 플레이어가 몬스터의 인식 범위 안으로 진입했을 경우 바로 추적으로 들어감.
	if (fLen < pMonster->GetInfo().fRecogRange)
	{
		ChangeAIState(GetAI(), MON_STATE::TRACE);
	}

	////// 지정된 대기 시간만큼 대기했다면 이동한다.
	if (m_fWaitTime <= 0.f)
	{
		ChangeAIState(GetAI(), MON_STATE::PATROL);
	}
}
