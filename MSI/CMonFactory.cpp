#include "global.h"
#include "CMonFactory.h"
#include "CMonster.h"
#include "CMonster_Normal.h"
#include "AI.h"

#include "CIdleState.h"
#include "CTraceState.h"
#include "CPatrolState.h"
#include "CAttackState.h"
#include "CDeadState.h"

#include "CComponent.h"
#include "CRigidBody.h"

CMonster* CMonFactory::CreateMonster(MON_TYPE _eType, Vec2 _vPos)
{
	CMonster* pMon = nullptr;

	switch (_eType)
	{
	case MON_TYPE::NORMAL:
	{
		pMon = new CMonster_Normal;
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
		pAI->AddState(new CAttackState);
		pAI->AddState(new CDeadState);
		pAI->AddState(new CIdleState);
		pAI->AddState(new CPatrolState);
		pAI->AddState(new CTraceState);
		
		pAI->SetCurState(MON_STATE::IDLE);

		pMon->SetAI(pAI);
	}
		break;
	case MON_TYPE::EPIC:
	{
	}
		break;
	}

	assert(pMon);

	return pMon;
}

CMonster* CMonFactory::CreateMonster(MON_TYPE _eType
	, wstring _strName, Vec2 _vPos, Vec2 _vScale
	, bool _bCollider, Vec2 _vColOffset, Vec2 _vColScale
	, bool _bAnimation, vector<wstring> _vecPath
	, bool _bGravity, bool _bRigidBody)
{
	CMonster* pMon = nullptr;

	switch (_eType)
	{
	case MON_TYPE::PUPPET:
	{
		pMon = new CMonster_Normal(_strName, _vPos, _vScale, _bCollider, _vColOffset, _vColScale, _bAnimation, _vecPath, _bGravity, _bRigidBody);

		tMonInfo info = {};
		info.fAtt = 10.f;
		info.fAttRange = 50.f;
		info.vRecogRange = Vec2(250.f, 50.f);
		info.fRecogRange = 300.f;
		info.fHP = 100.f;
		info.fSpeed = 300.f;

		pMon->SetMonInfo(info);

		if (_bRigidBody)
		{
			pMon->GetComponent()->GetRigidbody()->SetMass(1.f);
		}
	}
	break;
	case MON_TYPE::NORMAL:
	{
		pMon = new CMonster_Normal(_strName, _vPos, _vScale, _bCollider, _vColOffset, _vColScale, _bAnimation, _vecPath, _bGravity, _bRigidBody);
		
		tMonInfo info = {};
		info.fAtt = 10.f;
		info.fAttRange = 50.f;
		info.vRecogRange = Vec2(250.f, 50.f);
		info.fRecogRange = 300.f;
		info.fHP = 100.f;
		info.fSpeed = 300.f;

		pMon->SetMonInfo(info);

		if (_bRigidBody)
		{
			pMon->GetComponent()->GetRigidbody()->SetMass(1.f);
		}

		AI* pAI = new AI;
		pAI->AddState(new CIdleState);
		pAI->AddState(new CTraceState);
		pAI->AddState(new CPatrolState);
		pAI->AddState(new CAttackState);
		pAI->AddState(new CDeadState);

		pAI->SetCurState(MON_STATE::IDLE);

		pMon->SetAI(pAI);
	}
	break;
	case MON_TYPE::EPIC:
	{
	}
	break;
	}

	assert(pMon);

	return pMon;
}
