#include "global.h"
#include "CTraceState.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CPlayer.h"
#include "CMonster.h"

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

CTraceState::CTraceState()
	: CState(MON_STATE::TRACE)
	, m_pDetect(nullptr)
{
}

CTraceState::~CTraceState()
{
}


void CTraceState::Enter()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	
	CMonster* pMon = GetMonster();
	Vec2 vPos = pMon->GetPos();
	Vec2 vScale = pMon->GetScale();

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
	// Trace State를 나갈 때, 해당 오브젝트를 삭제하도록 Event에 넣어주고,
	// m_pObj는 삭제된 주소값을 가리키기 때문에 nullptr로 변경한다.
	DeleteObject(m_pDetect);
	m_pDetect = nullptr;
}

void CTraceState::update()
{
	Vec2 vMonPos = GetMonster()->GetPos();
	CPlayer* pPlayer = (CPlayer*)CSceneMgr::GetInst()->GetCurScene()->GetPlayer();

	CCollider* pPlayerCol = pPlayer->GetComponent()->GetCollider();

	if(m_pDetect) m_pDetect->SetPos(vMonPos + Vec2(50.f * GetMonster()->GetInfo().iDir, 0.f));

	if (m_pDetect->IsDetect())
	{
		ChangeAIState(GetAI(), MON_STATE::ATT);
	}

	// 플레이어 추적
	HDC dc = CCore::GetInst()->GetMemTex()->GetDC();
	Vec2 vPlayerPos = pPlayer->GetPos();
	Vec2 vMonDir = vPlayerPos - vMonPos;
	if (vMonDir.IsZero()) return;


	vMonDir.Normalize();

	vMonPos += vMonDir * GetMonster()->GetInfo().fSpeed * fDT;

	GetMonster()->SetPos(vMonPos);
	GetMonster()->GetComponent()->GetRigidbody()->SetVelocity(Vec2((vPlayerPos - vMonPos).x, 0.f));

	// 플레이어가 탐지거리 이상으로 멀어진다면 추적을 멈춘다.
	CMonster* pMonster = GetMonster();
	Vec2 vDiff = (vPlayerPos - vMonPos);
	float fLen = vDiff.Length();

	//if (fLen > pMonster->GetInfo().vRecogRange)
	//{
	//	ChangeAIState(GetAI(), MON_STATE::IDLE);
	//}
}
