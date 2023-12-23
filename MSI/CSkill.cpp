#include "global.h"
#include "CSkill.h"

CSkill::CSkill(CObject* _pOwner, bool _bPassive)
	: m_pOwner(_pOwner)
	, m_bPassive(_bPassive)
{
}

CSkill::~CSkill()
{
}
