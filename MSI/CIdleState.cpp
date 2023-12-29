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
	// 1.5 ~ 3�� �������� ����ϴ� �ð��� ����.
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

	//// ������ ��� �ð���ŭ ����ߴٸ� �̵��Ѵ�.
	if (m_fWaitTime <= 0.f)
	{
		ChangeAIState(GetAI(), MON_STATE::PATROL);
	}

	CMonster_Normal* pMon = (CMonster_Normal*)GetMonster();
	if (pMon->GetInfo().fHP <= 0) ChangeAIState(GetAI(), MON_STATE::DEAD);

}


bool CIdleState::InRangePlayer()
{
	// Player�� ��ġ üũ
	CPlayer* pPlayer = (CPlayer*)CSceneMgr::GetInst()->GetCurScene()->GetPlayer();
	if (pPlayer == nullptr)
	{
		return false;
	}
	Vec2 vPlayerPos = pPlayer->GetPos();
	Vec2 vPlayerScale = pPlayer->GetScale();

	// ������ ���� �ȿ� ������ �������·� ��ȯ
	CMonster* pMonster = GetMonster();
	Vec2 vMonPos = pMonster->GetPos();
	Vec2 vMonScale = pMonster->GetScale();

	// ������ ���� ��ǥ
	float fMonLeftX = vMonPos.x - (vPlayerScale.x / 2.f);
	float fMonLeftY = vMonPos.y + (vMonScale.y / 2.f);

	// �÷��̾��� ���� ��ǥ
	float fPlayerLeftX = vPlayerPos.x - (vPlayerScale.x / 2.f);
	// �÷��̾��� ���� ��ǥ
	float fPlayerRightX = vPlayerPos.x + (vPlayerScale.x / 2.f);
	// �÷��̾��� �غ� ��ǥ
	float fPlayerY = vPlayerPos.y + (vPlayerScale.y / 2.f);

	// ������ �����Ÿ�
	float fMonRcogRangeX = pMonster->GetInfo().vRecogRange.x;

	// �÷��̾ ������ �������� ��쿡�� �������� ���Ƿ�,
	// ���� ������ ������ ũ�� ����� ����.
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
