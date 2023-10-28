#include "global.h"
#include "CMonster.h"

CMonster::CMonster()
	: m_fHP(0.f)
	, m_fSpeed(0.f)
	, m_fRecogRange(0.f)
	, m_fAttRange(0.f)
	, m_fAtt(0.f)
{
}

CMonster::~CMonster()
{
}

void CMonster::update()
{
	Vec2 vCurPos = GetPos();

}
