#include "global.h"

#include "CObject.h"

#include "CKeyMgr.h"
#include "CTimeMgr.h"

#include "CComponent.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CRigidBody.h"

CObject::CObject()
	: m_vPos{}
	, m_vScale{}
	, m_pComponent(nullptr)
	, m_bAlive(true)
{
}

// 복사 생성자
CObject::CObject(const CObject& _origin)
	: m_strName(_origin.m_strName)
	, m_vPos(_origin.m_vPos)
	, m_vScale(_origin.m_vScale)
	, m_pComponent(nullptr)
	, m_bAlive(true)
{
	if (_origin.m_pComponent)
	{
		m_pComponent = new CComponent(*_origin.m_pComponent);
		m_pComponent->GetCollider()->m_pOwner = this;
		m_pComponent->GetAnimator()->m_pOwner = this;
		m_pComponent->GetRigidbody()->m_pOwner = this;
		m_pComponent->m_pOwner = this;
	}
}

CObject::~CObject()
{
	if (m_pComponent != nullptr) delete m_pComponent;
}


void CObject::finalupdate()
{
	if (m_pComponent) m_pComponent->finalupdate();
}

void CObject::render(HDC _dc)
{
	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(m_vPos);

	RECT rc = {
	  (int)(vRenderPos.x - m_vScale.x / 2.f)
	, (int)((vRenderPos.y - m_vScale.y / 2.f) - 125.f)
	, (int)(vRenderPos.x + m_vScale.x / 2.f)
	, (int)(vRenderPos.y + m_vScale.y / 2.f) };

	DrawText(_dc, m_strName.c_str(), -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	Rectangle(_dc
		, (int)(vRenderPos.x - m_vScale.x / 2.f)
		, (int)(vRenderPos.y - m_vScale.y / 2.f)
		, (int)(vRenderPos.x + m_vScale.x / 2.f)
		, (int)(vRenderPos.y + m_vScale.y / 2.f));

	component_render(_dc);
}

void CObject::component_render(HDC _dc)
{
	if (m_pComponent->GetCollider() != nullptr) m_pComponent->GetCollider()->render(_dc);
	
	if (m_pComponent->GetAnimator() != nullptr) m_pComponent->GetAnimator()->render(_dc);

}


void CObject::CreateComponent()
{
	m_pComponent = new CComponent;
	m_pComponent->m_pOwner = this;
}

void CObject::CreateCollider()
{
	m_pComponent->m_pCollider = new CCollider;
	m_pComponent->m_pCollider->m_pOwner = this;
}

void CObject::CreateAnimation()
{
	m_pComponent->m_pAnimator = new CAnimator;
	m_pComponent->m_pAnimator->m_pOwner = this;
}

void CObject::CreateRigidbody()
{
	m_pComponent->m_pRigidbody = new CRigidBody;
	m_pComponent->m_pRigidbody->m_pOwner = this;
}
