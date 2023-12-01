#include "global.h"
#include "CMonFactory.h"
#include "CMonster.h"
#include "AI.h"

#include "CIdleState.h"
#include "CTraceState.h"
#include "CPatrolState.h"
#include "CAttackState.h"

#include "CComponent.h"
#include "CRigidBody.h"

CMonster* CMonFactory::CreateMonter(MON_TYPE _eType, Vec2 _vPos)
{
	CMonster* pMon = nullptr;

	switch (_eType)
	{
	case MON_TYPE::NORMAL:
	{
		pMon = new CMonster;
		pMon->SetPos(_vPos);

		tMonInfo info = {};
		info.fAtt = 10.f;
		info.fAttRange = 50.f;
		info.vRecogRange = Vec2(250.f, 50.f);
		info.fRecogRange = 300.f;
		info.fHP = 100.f;
		info.fSpeed = 300.f;

		pMon->SetMonInfo(info);

		pMon->CreateRigidbody();
		pMon->GetComponent()->GetRigidbody()->SetMass(1.f);

		AI* pAI = new AI;
		pAI->AddState(new CIdleState);
		pAI->AddState(new CTraceState);
		pAI->AddState(new CPatrolState);
		pAI->AddState(new CAttackState);
		
		pAI->SetCurState(MON_STATE::TRACE);

		pMon->SetAI(pAI);
	}
		break;
	case MON_TYPE::RANGE:
	{
	}
		break;
	}

	assert(pMon);

	return pMon;
}
