#include "global.h"
#include "CObject.h"

#include "CKeyMgr.h"
#include "CTimeMgr.h"

CObject::CObject()
{
}

CObject::~CObject()
{
}

void CObject::render(HDC _dc)
{
	RECT rc = {
	  (int)(m_vPos.x - m_vScale.x / 2.f)
	, (int)(m_vPos.y - m_vScale.y / 2.f) - 125.f
	, (int)(m_vPos.x + m_vScale.x / 2.f)
	, (int)(m_vPos.y + m_vScale.y / 2.f) };

	DrawText(_dc, m_strName.c_str(), -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	Rectangle(_dc
		, (int)(m_vPos.x - m_vScale.x / 2.f)
		, (int)(m_vPos.y - m_vScale.y / 2.f)
		, (int)(m_vPos.x + m_vScale.x / 2.f)
		, (int)(m_vPos.y + m_vScale.y / 2.f));
}
