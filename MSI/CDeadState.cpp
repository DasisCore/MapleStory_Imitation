#include "global.h"
#include "CDeadState.h"
#include "CTimeMgr.h"
#include "CMonster.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CPlayer.h"
#include "CSoundMgr.h"

CDeadState::CDeadState()
	: CState(MON_STATE::DEAD)
	, m_DeadDelay(1.0f)
{
	CSoundMgr::GetInst()->CreateSound(L"LeshDie", "LeshDie.mp3", false);
}

CDeadState::~CDeadState()
{
}

void CDeadState::Enter()
{
	// ������ ���� ��.
	CSoundMgr::GetInst()->Play(L"LeshDie", SOUND_TYPE::EFFECT2);
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
