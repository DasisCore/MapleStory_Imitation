#include "global.h"
#include "CPatrolState.h"

#include "CSceneMgr.h"
#include "CScene.h"

#include "CPlayer.h"
#include "CMonster.h"

#include "CRandom.h"
#include "CTimeMgr.h"

#include "CComponent.h"
#include "CRigidBody.h"


CPatrolState::CPatrolState()
	: CState(MON_STATE::PATROL)
	, m_fMoveTime(0.f)
	, m_iDir(1)
{
}

CPatrolState::~CPatrolState()
{
}

void CPatrolState::Enter()
{
	m_fMoveTime = CRandom::GetInst()->GetBetweenReal(1.f, 1.f);
	m_iDir = CRandom::GetInst()->GetRandomOne();
}

void CPatrolState::Exit()
{
}

void CPatrolState::update()
{
	CMonster* pMonster = GetMonster();
	CRigidBody* vMonRigid = pMonster->GetComponent()->GetRigidbody();

	float fMonScale = pMonster->GetScale().x;
	pMonster->SetMonDir(m_iDir);
	// 좌측 이동
	if (m_iDir == -1)
	{
		vMonRigid->SetVelocity(Vec2(-200.f, 0.f));
		if (pMonster->GetRemainDist().x < fMonScale)
		{
			if (CRandom::GetInst()->GetRandomBool())
			{
				vMonRigid->SetVelocity(Vec2(0.f, 0.f));
				ChangeAIState(GetAI(), MON_STATE::IDLE);
			}
			m_iDir = 1;
		}
	}
	// 우측 이동
	else if (m_iDir == 1)
	{
		vMonRigid->SetVelocity(Vec2(200.f, 0.f));
		if (pMonster->GetRemainDist().y < fMonScale)
		{
			if (CRandom::GetInst()->GetRandomBool())
			{
				vMonRigid->SetVelocity(Vec2(0.f, 0.f));
				ChangeAIState(GetAI(), MON_STATE::IDLE);
			}
			m_iDir = -1;
		}
	}


	if (m_fMoveTime <= 0.f)
	{
		ChangeAIState(GetAI(), MON_STATE::IDLE);
	}

	m_fMoveTime -= fDT;
}

