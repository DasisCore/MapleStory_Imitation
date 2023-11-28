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

CTraceState::CTraceState()
	: CState(MON_STATE::TRACE)
	, m_pObj(nullptr)
{
}

CTraceState::~CTraceState()
{
	//if (m_pObj) delete m_pObj;
}


void CTraceState::Enter()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	
	CMonster* pMon = GetMonster();
	Vec2 vPos = pMon->GetPos();

	if (m_pObj != nullptr)
	{
		m_pObj = new CMissile;
		m_pObj->SetScale(Vec2(10.f, 10.f));
		m_pObj->SetPos(vPos);
		m_pObj->SetName(L"miss");
		m_pObj->CreateComponent();
		m_pObj->CreateCollider();
		m_pObj->GetComponent()->GetCollider()->SetScale(Vec2(50.f, 50.f));
		m_pObj->GetComponent()->GetCollider()->SetOffsetPos(Vec2(50.f, 0.f));
		pCurScene->AddObject(m_pObj, GROUP_TYPE::MONSTER);
	}
}

void CTraceState::Exit()
{
	//if (m_pObj) delete m_pObj;
	if (m_pObj)
	{
		if (m_pObj->IsDead()) delete m_pObj;
		m_pObj->SetDead();
	};
}

void CTraceState::update()
{
	Vec2 vMonPos = GetMonster()->GetPos();

	if(m_pObj) m_pObj->SetPos(vMonPos);
	HDC dc = CCore::GetInst()->GetMemTex()->GetDC();
	
	CPlayer* pPlayer = (CPlayer*)CSceneMgr::GetInst()->GetCurScene()->GetPlayer();

	// 타겟 추적
	//CPlayer* pPlayer = (CPlayer*)CSceneMgr::GetInst()->GetCurScene()->GetPlayer();
	Vec2 vPlayerPos = pPlayer->GetPos();

	Vec2 vMonDir = vPlayerPos - vMonPos;
	if (vMonDir.IsZero()) return;

	vMonDir.Normalize();

	vMonPos += vMonDir * GetMonster()->GetInfo().fSpeed * fDT;

	GetMonster()->SetPos(vMonPos);

	// 플레이어가 탐지거리 이상으로 멀어진다면 추적을 멈춘다.
	CMonster* pMonster = GetMonster();
	Vec2 vDiff = (vPlayerPos - vMonPos);
	float fLen = vDiff.Length();

	if (fLen > pMonster->GetInfo().fRecogRange)
	{
		ChangeAIState(GetAI(), MON_STATE::IDLE);
	}
}
