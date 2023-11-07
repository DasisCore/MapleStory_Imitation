#include "global.h"

#include "CComponent.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CRigidBody.h"

CComponent::CComponent()
	: m_pOwner(nullptr)
	, m_pCollider(nullptr)
	, m_pAnimator(nullptr)
	, m_pRigidbody(nullptr)
{
}

CComponent::CComponent(const CComponent& _origin)
	: m_pOwner(nullptr)
	, m_pCollider(nullptr)
	, m_pAnimator(nullptr)
	, m_pRigidbody(nullptr)
{
	if (_origin.m_pCollider)
	{
		m_pCollider = new CCollider(*_origin.m_pCollider);
	}

	if (_origin.m_pAnimator)
	{
		m_pAnimator = new CAnimator(*_origin.m_pAnimator);
	}

	if (_origin.m_pRigidbody)
	{
		m_pRigidbody = new CRigidBody(*_origin.m_pRigidbody);
	}
}

CComponent::~CComponent()
{
	if (m_pCollider) delete m_pCollider;

	if (m_pAnimator) delete m_pAnimator;

	if (m_pRigidbody) delete m_pRigidbody;
}

void CComponent::finalupdate()
{
	if (m_pCollider) m_pCollider->finalupdate();
	if (m_pAnimator) m_pAnimator->finalupdate();
	if (m_pRigidbody) m_pRigidbody->finalupdate();
}
