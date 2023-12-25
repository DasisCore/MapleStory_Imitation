#include "global.h"
#include "CFortress.h"
#include "CTimeMgr.h"

#include "CPlayer.h"
#include "CComponent.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CTimeMgr.h"
#include "CMonster.h"

CFortress::CFortress(CObject* _pObj)
	: m_pOwner(_pObj)
	, m_fCastBegin(1.8f)
	, m_fCastRepeat(3.2f)
	, m_fCastEnd(1.6f)
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

CFortress::~CFortress()
{
}

void CFortress::CalCastingTime()
{
	CAnimator* pAni = GetComponent()->GetAnimator();

	if (m_fCastBegin > 0)
	{
		m_fCastBegin -= fDT;
	}

	if (m_fCastBegin < 0)
	{
		pAni->Play(L"FORTRESS_REPEAT", true);
		m_fCastRepeat -= fDT;
	}

	if (m_fCastRepeat < 0)
	{
		pAni->Play(L"FORTRESS_END", true);
		m_fCastEnd -= fDT;
	}

	if (m_fCastEnd < 0) DeleteObject(this);
}

void CFortress::update()
{
	// 위치 갱신
	SetPos(m_pOwner->GetPos());
		

	CalCastingTime();
}

void CFortress::OnCollisionEnter(CCollider* _pOther)
{
}

void CFortress::OnCollision(CCollider* _pOther)
{
}
