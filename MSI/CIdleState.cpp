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
	// 1.5 ~ 3�� �������� ����ϴ� �ð��� ����.
	m_fWaitTime = CRandom::GetInst()->GetBetweenReal(1.f, 2.f);
	GetMonster()->SetRandomDir();
}

void CIdleState::Exit()
{
}

void CIdleState::update()
{
	m_fWaitTime -= fDT;

	// Player�� ��ġ üũ
	CPlayer* pPlayer = (CPlayer*)CSceneMgr::GetInst()->GetCurScene()->GetPlayer();
	Vec2 vPlayerPos = pPlayer->GetPos();

	// ������ ���� �ȿ� ������ �������·� ��ȯ
	CMonster* pMonster = GetMonster();
	Vec2 vMonPos = pMonster->GetPos();

	Vec2 vDiff = (vPlayerPos - vMonPos);
	float fLen = vDiff.Length();

	// �÷��̾ ������ �ν� ���� ������ �������� ��� �ٷ� �������� ��.
	if (fLen < pMonster->GetInfo().fRecogRange)
	{
		ChangeAIState(GetAI(), MON_STATE::TRACE);
	}

	////// ������ ��� �ð���ŭ ����ߴٸ� �̵��Ѵ�.
	if (m_fWaitTime <= 0.f)
	{
		ChangeAIState(GetAI(), MON_STATE::PATROL);
	}
}
