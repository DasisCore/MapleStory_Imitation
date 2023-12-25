#include "global.h"
#include "CBlessingSeed.h"
#include "CTimeMgr.h"

#include "CComponent.h"
#include "CCollider.h"
#include "CAnimator.h"

CBlessingSeed::CBlessingSeed(CObject* _pObj)
	: m_pOwner(_pObj)
	, m_fCastTime(2.f)
{
	CreateComponent();
	CreateCollider();

	CCollider* pCol = GetComponent()->GetCollider();
	pCol->SetScale(Vec2(580.f, 300.f));
	pCol->SetOffsetPos(Vec2(0.f, -100.f));

	CreateAnimator();
	CAnimator* pAni = GetComponent()->GetAnimator();
	pAni->LoadAnimation(L"Animation\\FORTRESS_BEGIN.anim");		// 9 프레임
	pAni->LoadAnimation(L"Animation\\FORTRESS_REPEAT.anim");	// 16 프레임
	pAni->LoadAnimation(L"Animation\\FORTRESS_END.anim");		// 8 프레임
	pAni->Play(L"FORTRESS_BEGIN", false);
}

CBlessingSeed::~CBlessingSeed()
{
}


void CBlessingSeed::CalCastingTime()
{
	if (m_fCastTime < 0) DeleteObject(this);

	m_fCastTime -= fDT;
}

void CBlessingSeed::update()
{
	SetPos(m_pOwner->GetPos());

	CalCastingTime();
}

void CBlessingSeed::OnCollisionEnter(CCollider* _pOther)
{
}

void CBlessingSeed::OnCollision(CCollider* _pOther)
{
}

