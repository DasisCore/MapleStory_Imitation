#include "global.h"
#include "CSeed.h"
#include "CTimeMgr.h"

#include "CComponent.h"
#include "CCollider.h"
#include "CAnimator.h"

CSeed::CSeed(CObject* _pObj)
	: m_pOwner(_pObj)
	, m_fCastBegin(0.54f)
	, m_fCastRepeat(3.f)
	, m_fCastEnd(0.42f)
{
	SetName(L"Seed");

	CreateComponent();
	CreateCollider();


	CCollider* pCol = GetComponent()->GetCollider();
	pCol->SetScale(Vec2(78.f, 100.f));
	pCol->SetOffsetPos(Vec2(0.f, -50.f));

	CreateAnimator();
	CAnimator* pAni = GetComponent()->GetAnimator();
	pAni->LoadAnimation(L"Animation\\SEED_BEGIN.anim");		// 9 프레임
	pAni->LoadAnimation(L"Animation\\SEED_REPEAT.anim");	// 16 프레임
	pAni->LoadAnimation(L"Animation\\SEED_END.anim");		// 8 프레임
	pAni->Play(L"SEED_BEGIN", true);
}

CSeed::~CSeed()
{
}

void CSeed::update()
{
	CalCastingTime();
}

void CSeed::CalCastingTime()
{
	CAnimator* pAni = GetComponent()->GetAnimator();

	if (m_fCastBegin > 0)
	{
		m_fCastBegin -= fDT;
	}
	else if (m_fCastBegin < 0 && m_fCastRepeat > 0)
	{
		pAni->Play(L"SEED_REPEAT", true);
		m_fCastRepeat -= fDT;
	}
	else if (m_fCastRepeat < 0 && m_fCastEnd > 0)
	{
		pAni->Play(L"SEED_END", true);
		m_fCastEnd -= fDT;
	}
	else if (m_fCastEnd < 0) DeleteObject(this);
}

void CSeed::OnCollisionEnter(CCollider* _pOther)
{
}

void CSeed::OnCollision(CCollider* _pOther)
{
}