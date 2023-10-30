#include "global.h"

#include "CComponent.h"
#include "CCollider.h"



CComponent::CComponent()
	: m_pOwner(nullptr)
	, m_pCollider(nullptr)
{
}

CComponent::CComponent(const CComponent& _origin)
	: m_pOwner(nullptr)
	, m_pCollider(nullptr)
{
	if (_origin.m_pCollider != nullptr)
	{
		m_pCollider = new CCollider(*_origin.m_pCollider);
	}
}

CComponent::~CComponent()
{
}

void CComponent::finalupdate()
{
	if (m_pCollider) m_pCollider->update();
}
