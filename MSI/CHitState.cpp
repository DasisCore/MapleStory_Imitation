#include "global.h"
#include "CHitState.h"
#include "CTimeMgr.h"

CHitState::CHitState()
	:CState(MON_STATE::HIT)
	, m_fHitTime(0.5f)
{
}

CHitState::~CHitState()
{
}


void CHitState::Enter()
{

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

