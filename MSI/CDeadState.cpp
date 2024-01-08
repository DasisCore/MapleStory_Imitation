#include "global.h"
#include "CDeadState.h"
#include "CTimeMgr.h"
#include "CMonster.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CPlayer.h"

CDeadState::CDeadState()
	: CState(MON_STATE::DEAD)
	, m_DeadDelay(2.2f)
{
}

CDeadState::~CDeadState()
{
}

void CDeadState::Enter()
{
	// ������ ���� ��.

}

void CDeadState::Exit()
{
}

void CDeadState::update()
{
	if (m_DeadDelay < 0)
	{
		GetMonster()->SetDead();
		
		CPlayer* pPlayer = (CPlayer*) CSceneMgr::GetInst()->GetCurScene()->GetPlayer();
		// ������ Ư�� ����ġ�� �ٲپ�� ��.

		CMonster* pMonster = GetMonster();
		int iMonExp = pMonster->GetInfo().iExp;
		pPlayer->AddExp(iMonExp);
	}

	m_DeadDelay -= fDT;
}
