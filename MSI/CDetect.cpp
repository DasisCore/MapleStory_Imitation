#include "global.h"
#include "CDetect.h"

#include "AI.h";

CDetect::CDetect()
	: m_bIsDetect(false)
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
}
