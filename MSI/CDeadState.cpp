#include "global.h"
#include "CDeadState.h"
#include "CTimeMgr.h"
#include "CMonster.h"

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
	}

	m_DeadDelay -= fDT;
}
