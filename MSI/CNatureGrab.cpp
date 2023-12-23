#include "global.h"
#include "CNatureGrab.h"

#include "CPlayer.h"
#include "CComponent.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CTimeMgr.h"
#include "CMonster.h"

CNatureGrab::CNatureGrab(CObject* _pObj)
	: m_pOwner(_pObj)
	, m_fCastTime(0.25f)
{
	CreateComponent();
	CreateCollider();
	
	CCollider* pCol = GetComponent()->GetCollider();
	pCol->SetScale(Vec2(580.f, 260.f));
	
	CreateAnimator();
	CAnimator* pAni = GetComponent()->GetAnimator();
	pAni->LoadAnimation(L"Animation\\NatureGrab.anim");
	pAni->Play(L"NatureGrab", false);
}

CNatureGrab::~CNatureGrab()
{


}

void CNatureGrab::CalCastingTime()
{
	if (m_fCastTime < 0) DeleteObject(this);
	m_fCastTime -= fDT;
}

void CNatureGrab::update()
{
	SetPos(m_pOwner->GetPos());

	// 스킬의 방향을 파악
	CPlayer* pPlayer = (CPlayer*)m_pOwner;
	m_iDir = pPlayer->GetDir();

	CCollider* pCol = GetComponent()->GetCollider();
	if (m_iDir == -1) pCol->SetOffsetPos(Vec2(-300.f, -80.f));
	else pCol->SetOffsetPos(Vec2(300.f, -80.f));

	CalCastingTime();
}

// 무조건 몬스터만 충돌판정으로 들어온다.
void CNatureGrab::OnCollisionEnter(CCollider* _pOther)
{
	CMonster* pMon = (CMonster*) _pOther->GetObj();
	pMon->TakeDamege(10.f);
}

void CNatureGrab::OnCollision(CCollider* _pOther)
{

}
