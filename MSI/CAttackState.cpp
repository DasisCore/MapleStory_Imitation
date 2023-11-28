#include "global.h"
#include "CAttackState.h"
#include "CCore.h"

#include "CDetect.h"
#include "CMonster.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CComponent.h"
#include "CCollider.h"
#include "CEventMgr.h"
#include "CLesh.h"

CAttackState::CAttackState()
	:CState(MON_STATE::ATT)
	, m_pDetect(nullptr)
{
}

CAttackState::~CAttackState()
{

}

void CAttackState::Enter()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	CMonster* pMon = GetMonster();
	Vec2 vPos = pMon->GetPos();

	if (m_pDetect == nullptr)
	{
		m_pDetect = new CDetect;
		m_pDetect->SetScale(Vec2(10.f, 10.f));
		m_pDetect->SetPos(vPos + Vec2(50.f, 0.f));
		m_pDetect->SetName(L"Detect_area");
		m_pDetect->CreateComponent();
		m_pDetect->CreateCollider();
		m_pDetect->GetComponent()->GetCollider()->SetScale(Vec2(50.f, 50.f));
		//m_pObj->GetComponent()->GetCollider()->SetOffsetPos(Vec2(50.f, 0.f));
		pCurScene->AddObject(m_pDetect, GROUP_TYPE::MONSTER);
	}

}

void CAttackState::Exit()
{
	DeleteObject(m_pDetect);
	m_pDetect = nullptr;
}

void CAttackState::update()
{
	CLesh* pMon = (CLesh*) GetMonster();
	pMon->Attack();

	if (pMon->IsFinshAtt())
	{
		ChangeAIState(GetAI(), MON_STATE::IDLE);
		pMon->SetAttTime();
	}
}

