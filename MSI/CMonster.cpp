#include "global.h"
#include "CMonster.h"

#include "CComponent.h"
#include "CCollider.h"

CMonster::CMonster()
	: m_fHP(0.f)
	, m_fSpeed(0.f)
	, m_fRecogRange(0.f)
	, m_fAttRange(0.f)
	, m_fAtt(0.f)
{
	CreateComponent();
	CreateCollider();
	GetComponent()->GetCollider()->SetScale(Vec2(110.f, 60.f));
}

CMonster::~CMonster()
{
}

void CMonster::update()
{
	Vec2 vCurPos = GetPos();

}
