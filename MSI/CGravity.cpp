#include "global.h"
#include "CGravity.h"

#include "CObject.h"
#include "CComponent.h"
#include "CRigidBody.h"

#include "CSceneMgr.h"
#include "CScene.h"

CGravity::CGravity()
	: m_pOwner(nullptr)
	, m_bGround(false)
{
}

CGravity::~CGravity()
{
}

void CGravity::SetGround(bool _b)
{
	m_bGround = _b;
	if (m_bGround)
	{
		Vec2 vV = m_pOwner->GetComponent()->GetRigidbody()->GetVelocity();
		m_pOwner->GetComponent()->GetRigidbody()->SetVelocity(Vec2(vV.x,  0.f));
	}
}

void CGravity::finalupdate()
{
	if (CSceneMgr::GetInst()->GetCurScene()->GetName() != L"Tool Scene")
	{
		m_pOwner->GetComponent()->GetRigidbody()->SetAccelAlpha(Vec2(0.f, 1500.f));	
	}
}
