#include "global.h"
#include "CDetect.h"
#include "CCollider.h"

#include "AI.h";

CDetect::CDetect()
	: m_bIsDetect(false)
	, m_pObject(nullptr)
{
}

CDetect::~CDetect()
{
}

void CDetect::update()
{

}

void CDetect::OnCollisionEnter(CCollider* _pOther)
{
	m_bIsDetect = true;
	m_pObject = _pOther->GetObj();
}

void CDetect::OnCollisionExit(CCollider* _pOther)
{
	m_bIsDetect = false;
	m_pObject = nullptr;
}
