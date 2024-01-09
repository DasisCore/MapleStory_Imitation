#include "global.h"
#include "CAttackState.h"
#include "CCore.h"

#include "CDetect.h"
#include "CMonster.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CComponent.h"
#include "CCollider.h"
#include "CPlayer.h"
#include "CEventMgr.h"
#include "CMonster_Normal.h"
#include "CTimeMgr.h"

CAttackState::CAttackState()
	:CState(MON_STATE::ATT)
	, m_pDetect(nullptr)
	, m_fAttack(0.73f)
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

	// ���� ���� �ȿ� ���� ���. �÷��̾�� �˹��� �ش�.
	// �÷��̾��� ���� ���� �� �����ؾ� ��.
	if (m_pDetect->IsDetect())
	{
		m_fAttack -= fDT;

		// ���� ��ǿ� ���߾� ����
		if (m_fAttack < 0 && !m_bAttack)
		{
			CObject* pObj = m_pDetect->GetDetectObj();
			CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);
				
			// �÷��̾ �ƴ϶�� nullptr�� ��ȯ
			if (pPlayer != nullptr)
			{
				int iMonDir = pMon->GetInfo().iDir;
				// ���� ���⿡ ���� hit ������ ����.
				pPlayer->CharHit(iMonDir, int(pMonster->GetInfo().fAtt));
				m_bAttack = true;
				ChangeAIState(GetAI(), MON_STATE::TRACE);
			}
		}
	}
	else
	{
		m_fAttack = 0.73f;
		m_bAttack = false;
	}

	if (pMon->IsFinshAtt())
	{
		pMon->SetAttTime();
		m_bAttack = false;
		ChangeAIState(GetAI(), MON_STATE::TRACE);
		return;
	}

	if (pMon->GetInfo().fHP <= 0)
	{
		ChangeAIState(GetAI(), MON_STATE::DEAD);
		return;
	}
}

