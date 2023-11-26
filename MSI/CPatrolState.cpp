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
{
}

CPatrolState::~CPatrolState()
{
}

void CPatrolState::Enter()
{
	m_fMoveTime = CRandom::GetInst()->GetBetweenReal(1.f, 1.f);
}

void CPatrolState::Exit()
{
}

void CPatrolState::update()
{
	CMonster* pMonster = GetMonster();
	CRigidBody* vMonRigid = pMonster->GetComponent()->GetRigidbody();

	// 좌측 이동
	if (pMonster->GetInfo().mDir < 0)
	{
		vMonRigid->SetVelocity(Vec2(-200.f, 0.f));

		//if (vMonRigid->GetVelocity() == Vec2(0.f, 0.f))
		//{
		//	vMonRigid->SetVelocity(Vec2(-200.f, 0.f));
		//}
		//else
		//{
		//	vMonRigid->AddForce(Vec2(-100.f, 0.f));
		//}
	}
	// 우측 이동
	else
	{
		vMonRigid->SetVelocity(Vec2(200.f, 0.f));

		//if (vMonRigid->GetVelocity() == Vec2(0.f, 0.f))
		//{
		//	vMonRigid->SetVelocity(Vec2(200.f, 0.f));
		//}
		//else
		//{
		//	vMonRigid->AddForce(Vec2(100.f, 0.f));
		//}
	}


	if (m_fMoveTime <= 0.f)
	{
		ChangeAIState(GetAI(), MON_STATE::IDLE);
	}

	m_fMoveTime -= fDT;
}

