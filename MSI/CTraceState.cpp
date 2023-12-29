#include "global.h"
#include "CTraceState.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CMonster_Normal.h"

#include "CTimeMgr.h"
#include "CCollider.h"
#include "CCore.h"
#include "CTexture.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CComponent.h"
#include "CObject.h"
#include "CMissile.h"
#include "CDetect.h"
#include "CEventMgr.h"
#include "CRigidBody.h"
#include "CTimeMgr.h"

CTraceState::CTraceState()
	: CState(MON_STATE::TRACE)
	, m_pDetect(nullptr)
	, m_fTraceTime(0.8f)
	, m_fRemainTraceTime(3.f)
	, m_iDir(1)
{
}

CTraceState::~CTraceState()
{
	//if (m_pDetect) DeleteObject(m_pDetect);
}

void CTraceState::Enter()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	
	CMonster* pMon = GetMonster();
	Vec2 vPos = pMon->GetPos();
	Vec2 vScale = pMon->GetScale();
	m_fRemainTraceTime = 3.f;
	m_fTraceTime = 0.8f;

	// �÷��̾�� ������ �Ÿ��� ����ؼ� ������ ������ �����Ѵ�.
	Vec2 vMonPos = pMon->GetPos();
	CPlayer* pPlayer = (CPlayer*)CSceneMgr::GetInst()->GetCurScene()->GetPlayer();
	Vec2 vPlayerPos = pPlayer->GetPos();
	Vec2 vMonDir = vPlayerPos - vMonPos;

	if (vMonDir.x >= 0)
	{
		m_iDir = 1;
		pMon->SetMonDir(1);
	}
	else
	{
		m_iDir = -1;
		pMon->SetMonDir(-1);
	}

	if (m_pDetect == nullptr)
	{
		m_pDetect = new CDetect;
		m_pDetect->SetScale(Vec2(10.f, 10.f));
		m_pDetect->SetPos(vPos + Vec2(50.f, 0.f));
		m_pDetect->SetName(L"Detect_area");
		m_pDetect->SetGroundCheck(false);
		m_pDetect->CreateComponent();
		m_pDetect->CreateCollider();
		m_pDetect->GetComponent()->GetCollider()->SetScale(vScale);
		//m_pDetect->GetComponent()->GetCollider()->SetScale(Vec2(100.f, 50.f));
		//m_pObj->GetComponent()->GetCollider()->SetOffsetPos(Vec2(50.f, 0.f));
		pCurScene->AddObject(m_pDetect, GROUP_TYPE::DETECT);
	}
}

void CTraceState::Exit()
{
	// Trace State�� ���� ��, �ش� ������Ʈ�� �����ϵ��� Event�� �־��ְ�,
	// m_pObj�� ������ �ּҰ��� ����Ű�� ������ nullptr�� �����Ѵ�.
	DeleteObject(m_pDetect);
	m_pDetect = nullptr;
}

void CTraceState::update()
{
	CMonster* pMonster = GetMonster();
	Vec2 vMonPos = pMonster->GetPos();

	CPlayer* pPlayer = (CPlayer*)CSceneMgr::GetInst()->GetCurScene()->GetPlayer();
	CCollider* pPlayerCol = pPlayer->GetComponent()->GetCollider();

	Vec2 vPlayerPos = pPlayer->GetPos();
	Vec2 vMonDir = vPlayerPos - vMonPos;

	// Ž�� �簢�� ��ġ ����
	if(m_pDetect) m_pDetect->SetPos(vMonPos + Vec2(50.f * GetMonster()->GetInfo().iDir, 0.f));

	// ���� ������ �÷��̾ ���Դٸ� �������� ��ȯ
	if (m_pDetect->IsDetect())
	{
		pMonster->GetComponent()->GetRigidbody()->SetVelocity(Vec2((float)m_iDir, 0.f));
		ChangeAIState(GetAI(), MON_STATE::ATT);
		return;
	}

	// =========================================================
	// �÷��̾ ���� ���� ��,
	// =========================================================

	// ������ �� ����Ŭ�� �����ٸ�
	if (m_fTraceTime < 0)
	{
		m_fTraceTime = 0.8f;
		if (vMonDir.x >= 0)
		{
			pMonster->SetMonDir(1);
			m_iDir = 1;
		}
		else if (vMonDir.x < 0)
		{
			pMonster->SetMonDir(-1);
			m_iDir = -1;
		}
	}

	CRigidBody* pMonRigid = pMonster->GetComponent()->GetRigidbody();

	float vRemainLeft = pMonster->GetRemainDist().x;
	float vRemainRight = pMonster->GetRemainDist().y;

	float speed = pMonster->GetSpeed();

	if (m_iDir == -1)
	{
		pMonRigid->SetVelocity(Vec2(speed * m_iDir , 0.f));

		if (vRemainLeft < pMonster->GetScale().x)
		{
			m_iDir = 1;
			pMonster->SetMonDir(1);
			pMonRigid->SetVelocity(Vec2(speed * m_iDir, 0.f));
		}
	}
	else if (m_iDir == 1)
	{
		pMonRigid->SetVelocity(Vec2(speed * m_iDir, 0.f));

		if (vRemainRight < pMonster->GetScale().x)
		{
			m_iDir = -1;
			pMonster->SetMonDir(-1);
			pMonRigid->SetVelocity(Vec2(speed * m_iDir, 0.f));
		}
	}

	m_fTraceTime -= fDT;
	
	// ������ġ (Detect�� ���� �ɸ� ������ ����.)
	if (vMonDir.IsZero()) return;

	// �÷��̾ Ž���Ÿ� �̻����� �־����ٸ� ������ �����.
	//CMonster* pMonster = GetMonster();
	Vec2 vDiff = (vPlayerPos - vMonPos);
	float fLen = vDiff.Length();

	// �÷��̾ Ž�� �Ÿ� �̻����� �־����ٸ�
	if (fLen > pMonster->GetInfo().fRecogRange)
	{
		m_fRemainTraceTime -= fDT;

		if (m_fRemainTraceTime < 0)
		{
			ChangeAIState(GetAI(), MON_STATE::IDLE);
		}
	}
	else
	{
		m_fRemainTraceTime = 3.f;
	}

	CMonster_Normal* pMon = (CMonster_Normal*)GetMonster();
	if (pMon->GetInfo().fHP <= 0) ChangeAIState(GetAI(), MON_STATE::DEAD);
}
