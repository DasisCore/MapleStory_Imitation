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
	, m_fCastTime(0.5f)
{
	CreateComponent();
	CreateCollider();
	
	CCollider* pCol = GetComponent()->GetCollider();
	pCol->SetScale(Vec2(580.f, 280.f));
	
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
	if (m_iDir == -1) pCol->SetOffsetPos(Vec2(-310.f, -100.f));
	else pCol->SetOffsetPos(Vec2(310.f, -100.f));

	CalCastingTime();
}

// 무조건 몬스터만 충돌판정으로 들어온다.
void CNatureGrab::OnCollisionEnter(CCollider* _pOther)
{
	CMonster* pMon = dynamic_cast<CMonster*>(_pOther->GetObj());
	if (pMon != nullptr && pMon->GetInfo().fHP > 0)
	{
		pMon->TakeDamege(60.f, m_iDir);
	}
}

void CNatureGrab::OnCollision(CCollider* _pOther)
{

}
