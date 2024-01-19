#include "global.h"
#include "CHitState.h"
#include "CTimeMgr.h"
#include "CSoundMgr.h"

CHitState::CHitState()
	:CState(MON_STATE::HIT)
	, m_fHitTime(0.5f)
{
	CSoundMgr::GetInst()->CreateSound(L"LeshDamege", "LeshDamege.mp3", false);
}

CHitState::~CHitState()
{
}


void CHitState::Enter()
{
	CSoundMgr::GetInst()->Play(L"LeshDamege", SOUND_TYPE::EFFECT1);
}

void CHitState::Exit()
{
}

void CHitState::update()
{
	m_fHitTime -= fDT;

	if (m_fHitTime < 0)
	{
		ChangeAIState(GetAI(), MON_STATE::TRACE);
		return;
	}
}

