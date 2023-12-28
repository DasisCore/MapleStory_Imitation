#include "global.h"

#include "CObject.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"

#include "CComponent.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CRigidBody.h"
#include "CGravity.h"

#include "SelectGDI.h"

CObject::CObject()
	: m_vPos{}
	, m_vScale{}
	, m_pComponent(nullptr)
	, m_bAlive(true)
	, m_bGroundCheck(true)
	, m_bTarget(false)
{
}

// 복사 생성자
CObject::CObject(const CObject& _origin)
	: m_strName(_origin.m_strName)
	, m_vPos(_origin.m_vPos)
	, m_vScale(_origin.m_vScale)
	, m_pComponent(nullptr)
	, m_bAlive(true)
	, m_bGroundCheck(_origin.m_bGroundCheck)
{
	if (_origin.m_pComponent)
	{
		m_pComponent = new CComponent(*_origin.m_pComponent);
		m_pComponent->GetCollider()->m_pOwner = this;
		m_pComponent->GetAnimator()->m_pOwner = this;
		m_pComponent->GetRigidbody()->m_pOwner = this;
		m_pComponent->GetGravity()->m_pOwner = this;
		m_pComponent->m_pOwner = this;
	}
}

CObject::~CObject()
{
	if (m_pComponent != nullptr) delete m_pComponent;
}


void CObject::finalupdate()
{
	//CheckLocation();
	if (CSceneMgr::GetInst()->GetCurScene()->GetName() == L"Tool Scene")
	//if (1)
	{
		ObjectDrag();
		if (m_bTarget && KEY_TAP(KEY::DEL)) DeleteObject(this);
	}

	if (m_pComponent) m_pComponent->finalupdate();
}

void CObject::render(HDC _dc)
{
	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(m_vPos);

	// 이름표 출력
	Graphics graphics(_dc);

	Font font(L"Arial", 13, FontStyle::FontStyleBold);

	SolidBrush brush(Color(255, 0, 0, 0));

	Vec2 vPos = GetPos();
	vPos = CCamera::GetInst()->GetRenderPos(vPos);

	PointF point((int)(vRenderPos.x - m_vScale.x / 2.f), vPos.y - 70.f);

	graphics.DrawString(m_strName.c_str(), -1, &font, point, &brush);
	
	component_render(_dc);
}

void CObject::component_render(HDC _dc)
{
	if (m_pComponent)
	{
		if (m_pComponent->GetAnimator() != nullptr) m_pComponent->GetAnimator()->render(_dc);

		if (m_pComponent->GetCollider() != nullptr) m_pComponent->GetCollider()->render(_dc);
	}
}



void CObject::ObjectDrag()
{
	//if (m_bTarget)
	//{
	//	if (KEY_HOLD(KEY::LBTN))
	//	{
	//		Vec2 vCurPos = GetPos();
	//		Vec2 vDiff = CCamera::GetInst()->GetRealPos(MOUSE_POS) - CKeyMgr::GetInst()->GetDragStart();

	//		vCurPos += vDiff;
	//		SetPos(vCurPos);
	//		m_vDragStart = CCamera::GetInst()->GetRealPos(MOUSE_POS);
	//	}
	//}

	//if (CKeyMgr::GetInst()->IsMouseInObj(this))
	//{
		//if (KEY_TAP(KEY::LBTN))
		//{
		//	if (CKeyMgr::GetInst()->IsMouseInObj(this))
		//	{
		//		m_vDragStart = CCamera::GetInst()->GetRealPos(MOUSE_POS);
		//		m_bTarget = true;
		//	}
		//}

		//else if (m_bTarget && KEY_HOLD(KEY::LBTN))
		//{
		//	if (CKeyMgr::GetInst()->IsMouseInObj(this))
		//	{
		//		Vec2 vCurPos = GetPos();
		//		Vec2 vDiff = CCamera::GetInst()->GetRealPos(MOUSE_POS) - m_vDragStart;

		//		vCurPos += vDiff;
		//		SetPos(vCurPos);
		//		m_vDragStart = CCamera::GetInst()->GetRealPos(MOUSE_POS);
		//	}
		//}

		//else if (KEY_AWAY(KEY::LBTN))
		//{
		//	if (CKeyMgr::GetInst()->IsMouseInObj(this))
		//	{
		//		m_bTarget = false;
		//	}
		//}
	//}
}

void CObject::CheckLocation()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	Vec2 vMapSize = pCurScene->GetMapSize();

	if (vMapSize != Vec2(0.f, 0.f))
	{
		Vec2 vPos = GetPos();
		Vec2 vScale = GetScale();

		Vec2 vLT = Vec2(vPos.x - (vScale.x / 2.f), vPos.y - (vScale.y / 2.f));
		Vec2 vRB = Vec2(vPos.x + (vScale.x / 2.f), vPos.y + (vScale.y / 2.f));

		if (vPos.x <= 0) vPos.x = vScale.x;
		if (vPos.x >= vMapSize.x) vPos.x = vMapSize.x - (vScale.x / 2.f);
		if (vPos.y <= 0) vPos.y = (vScale.y / 2.f);
		if (vPos.y >= vMapSize.y) vPos.y = vMapSize.y - (vScale.y / 2.f);
		SetPos(vPos);
	}
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

void CObject::CreateAnimator()
{
	m_pComponent->m_pAnimator = new CAnimator;
	m_pComponent->m_pAnimator->m_pOwner = this;
}

void CObject::CreateRigidbody()
{
	m_pComponent->m_pRigidbody = new CRigidBody;
	m_pComponent->m_pRigidbody->m_pOwner = this;
}

void CObject::CreateGravity()
{
	m_pComponent->m_pGravity = new CGravity;
	m_pComponent->m_pGravity->m_pOwner = this;
}
