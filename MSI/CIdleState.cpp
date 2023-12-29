#include "global.h"
#include "CIdleState.h"

#include "CSceneMgr.h"
#include "CScene.h"

#include "CPlayer.h"
#include "CMonster.h"
#include "CMonster_Normal.h"
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
	//GetMonster()->SetRandomDir();
}

void CIdleState::Exit()
{
}

void CIdleState::update()
{
	m_fWaitTime -= fDT;

	if (InRangePlayer())
	{
		ChangeAIState(GetAI(), MON_STATE::TRACE);
	}

	//// 지정된 대기 시간만큼 대기했다면 이동한다.
	if (m_fWaitTime <= 0.f)
	{
		ChangeAIState(GetAI(), MON_STATE::PATROL);
	}

	CMonster_Normal* pMon = (CMonster_Normal*)GetMonster();
	if (pMon->GetInfo().fHP <= 0) ChangeAIState(GetAI(), MON_STATE::DEAD);

}


bool CIdleState::InRangePlayer()
{
	// Player의 위치 체크
	CPlayer* pPlayer = (CPlayer*)CSceneMgr::GetInst()->GetCurScene()->GetPlayer();
	if (pPlayer == nullptr)
	{
		return false;
	}
	Vec2 vPlayerPos = pPlayer->GetPos();
	Vec2 vPlayerScale = pPlayer->GetScale();

	// 몬스터의 범위 안에 들어오면 추적상태로 변환
	CMonster* pMonster = GetMonster();
	Vec2 vMonPos = pMonster->GetPos();
	Vec2 vMonScale = pMonster->GetScale();

	// 몬스터의 우측 좌표
	float fMonLeftX = vMonPos.x - (vPlayerScale.x / 2.f);
	float fMonLeftY = vMonPos.y + (vMonScale.y / 2.f);

	// 플레이어의 좌측 좌표
	float fPlayerLeftX = vPlayerPos.x - (vPlayerScale.x / 2.f);
	// 플레이어의 우측 좌표
	float fPlayerRightX = vPlayerPos.x + (vPlayerScale.x / 2.f);
	// 플레이어의 밑변 좌표
	float fPlayerY = vPlayerPos.y + (vPlayerScale.y / 2.f);

	// 몬스터의 사정거리
	float fMonRcogRangeX = pMonster->GetInfo().vRecogRange.x;

	// 플레이어가 완전히 근접했을 경우에는 공격으로 들어가므로,
	// 근접 상태의 오차는 크게 상관이 없다.
	if (pMonster->GetInfo().iDir == 1)
	{
		if (vMonPos.x < fPlayerLeftX && fPlayerLeftX < vMonPos.x + fMonRcogRangeX)
		{
			if (fMonLeftY >= fPlayerY && fPlayerY >= fMonLeftY - vMonScale.y)
			{
				return true;
			}
		}
	}
	else if (pMonster->GetInfo().iDir == -1)
	{
		if (vMonPos.x > fPlayerRightX && fPlayerRightX > vMonPos.x - fMonRcogRangeX)
		{
			if (fMonLeftY >= fPlayerY && fPlayerY >= fMonLeftY - vMonScale.y)
			{
				return true;
			}
		}
	}

	return false;
}
