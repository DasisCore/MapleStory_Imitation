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
#include "CMonster_Normal.h"

CAttackState::CAttackState()
	:CState(MON_STATE::ATT)
	, m_pDetect(nullptr)
{
}

CAttackState::~CAttackState()
{
	//if(m_pDetect) DeleteObject(m_pDetect);
}

void CAttackState::Enter()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	CMonster* pMon = GetMonster();
	Vec2 vMonPos = pMon->GetPos();

	pMon->GetInfo().iDir;

	if (m_pDetect == nullptr)
	{
		m_pDetect = new CDetect;
		m_pDetect->SetScale(Vec2(10.f, 10.f));
		m_pDetect->SetPos(vMonPos + Vec2(50.f * GetMonster()->GetInfo().iDir, 0.f));
		m_pDetect->SetName(L"Detect_area");
		m_pDetect->CreateComponent();
		m_pDetect->CreateCollider();
		m_pDetect->GetComponent()->GetCollider()->SetScale(pMon->GetScale());
		//m_pObj->GetComponent()->GetCollider()->SetOffsetPos(Vec2(50.f, 0.f));
		pCurScene->AddObject(m_pDetect, GROUP_TYPE::DETECT);
	}

}

void CAttackState::Exit()
{
	if (m_pDetect) DeleteObject(m_pDetect);
	m_pDetect = nullptr;
}

void CAttackState::update()
{
	CMonster* pMonster = GetMonster();
	CMonster_Normal* pMon = (CMonster_Normal*) GetMonster();
	Vec2 vMonPos = pMon->GetPos();
	if (m_pDetect) m_pDetect->SetPos(vMonPos + Vec2(50.f * GetMonster()->GetInfo().iDir, 0.f));
	
	pMon->Attack();

	if (pMon->IsFinshAtt())
	{
		pMon->SetAttTime();
		ChangeAIState(GetAI(), MON_STATE::IDLE);
		return;
	}

	if (pMon->GetInfo().fHP < 0) ChangeAIState(GetAI(), MON_STATE::DEAD);
}

