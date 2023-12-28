#include "global.h"
#include "CFoothold.h"

#include "CKeyMgr.h"
#include "CComponent.h"
#include "CCollider.h"
#include "CGravity.h"
#include "CMonster.h"
#include "CAnimator.h"

#include "CRigidBody.h"



CFoothold::CFoothold()
	: m_bLeft(false)
	, m_bTop(true)
	, m_bRight(false)
	, m_bBottom(false)
{
	CreateComponent();
	CreateCollider();
}

CFoothold::CFoothold(wstring _strName, Vec2 _vPos, Vec2 _vScale, bool _bCollider, Vec2 _vColOffset, Vec2 _vColScale, bool _bAnimation, vector<wstring> _vecPath, bool _bGravity, bool _bRigidBody)
	: m_bLeft(false)
	, m_bTop(true)
	, m_bRight(false)
	, m_bBottom(false)
{
	SetName(_strName);
	SetPos(_vPos);
	SetScale(_vScale);

	CreateComponent();

	if (_bCollider)
	{
		CreateCollider();
		CCollider* pCol = GetComponent()->GetCollider();
		pCol->SetOffsetPos(_vColOffset);
		pCol->SetScale(_vColScale);
	}

	if (_bAnimation)
	{
		CreateAnimator();
		CAnimator* pAnimator = GetComponent()->GetAnimator();
		for (int i = 0; i < _vecPath.size(); i++) pAnimator->LoadAnimation(_vecPath[i]);
		pAnimator->Play(_strName, 1);
	}

	if (_bGravity) CreateGravity();
	if (_bRigidBody) CreateRigidbody();
}

CFoothold::~CFoothold()
{
}

void CFoothold::start()
{
	// 기본적으로 자기 자신만큼의 크기를 가지도록 함.
	if (GetComponent()->GetCollider() != nullptr)
	{
		GetComponent()->GetCollider()->SetScale(Vec2(GetScale()));
	}
}

void CFoothold::update()
{
	ChangeBlock();
}

void CFoothold::render(HDC _dc)
{
	Graphics graphics(_dc);

	Vec2 vPos = GetPos();
	vPos = CCamera::GetInst()->GetRenderPos(vPos);
	Vec2 vScale = GetScale();

	Rect rectangle(vPos.x - (vScale.x / 2.f), vPos.y - (vScale.y / 2.f), vScale.x, vScale.y);

	Pen pen1(Color(0, 0, 0), 3);
	Pen pen2(Color(240, 240, 240), 3);
	if (GetTarget()) pen1.SetColor(Color(80, 140, 239));

	PointF leftTop(rectangle.X, rectangle.Y);
	PointF rightTop(rectangle.GetRight(), rectangle.Y);
	PointF leftBottom(rectangle.X, rectangle.GetBottom());
	PointF rightBottom(rectangle.GetRight(), rectangle.GetBottom());
	
	graphics.DrawLine(&pen2, leftTop, rightTop);
	graphics.DrawLine(&pen2, rightTop, rightBottom);
	graphics.DrawLine(&pen2, rightBottom, leftBottom);
	graphics.DrawLine(&pen2, leftTop, leftBottom);

	if (m_bLeft) graphics.DrawLine(&pen1, leftTop, leftBottom);
	if (m_bTop) graphics.DrawLine(&pen1, leftTop, rightTop);
	if (m_bRight) graphics.DrawLine(&pen1, rightTop, rightBottom);
	if (m_bBottom) graphics.DrawLine(&pen1, rightBottom, leftBottom);

	Font font(L"Arial", 8);
	SolidBrush brush(Color(0, 0, 0));
	graphics.DrawString(GetName().c_str(), -1, &font, PointF(vPos.x, vPos.y), &brush);
}

void CFoothold::OnCollisionEnter(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetGroundCheck())
	{
		Blocking(_pOther);
	}
}

void CFoothold::OnCollision(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetGroundCheck())
	{
		//if (_pOther->GetObj()->GetName().substr(0, 7) == L"Monster")
		if (1)
		{
			// 몬스터의 정보
			float fMonPos = _pOther->GetFinalPos().x;
			float fMonHalfSide = _pOther->GetScale().x / 2;

			// 땅의 정보
			float fGroundPos = GetPos().x;
			float fGroundHalfSide = GetScale().x / 2;

			// 남은 거리를 계속 계산해줘야 함. (좌우)

			CMonster* pMonster = (CMonster*)_pOther->GetObj();

			float fLeftRemain = abs((fMonPos + fMonHalfSide) - (fGroundPos - fGroundHalfSide));
			float fRightRemain = abs((fMonPos - fMonHalfSide) - (fGroundPos + fGroundHalfSide));
			pMonster->SetRemainDist(fLeftRemain, fRightRemain);
		}

		Blocking(_pOther);
	}
}

void CFoothold::OnCollisionExit(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetGroundCheck())
	{
		pOtherObj->GetComponent()->GetGravity()->SetGround(false);
	}
}

void CFoothold::Blocking(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();

	// 상대 콜라이더의 위치, 크기 파악
	Vec2 vObjPos = _pOther->GetFinalPos();
	Vec2 vObjScale = _pOther->GetScale();

	// 나의 위치와 크기 파악
	Vec2 vPos = GetComponent()->GetCollider()->GetFinalPos();
	Vec2 vScale = GetComponent()->GetCollider()->GetScale();

	CRigidBody* pRigid = pOtherObj->GetComponent()->GetRigidbody();
	Vec2 vCurVelocity = pRigid->GetVelocity();


	if (pRigid->GetVelocity().y < 0.f) return;

	// 상단
	if (int(vPos.y - (vScale.y / 2.f)) >= int(vObjPos.y + (vObjScale.y / 2.f)) - 3.f
		&& vPos.y - (vScale.y / 2.f) >= vObjPos.y - (vObjScale.y / 2.f)
		&& vPos.x - (vScale.x / 2.f) < vObjPos.x + (vObjScale.x / 2.f)
		&& vPos.x + (vScale.x / 2.f) > vObjPos.x - (vObjScale.x / 2.f)
		&& m_bTop)
	{
		if (pOtherObj->GetComponent()->GetGravity() != nullptr)
		{
			pOtherObj->GetComponent()->GetGravity()->SetGround(true);
		}
		float fLen = abs(vObjPos.y - vPos.y);
		float fValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;

		vObjPos = pOtherObj->GetPos();
		vObjPos.y -= fValue;

		pOtherObj->SetPos(vObjPos);
	}
	// 하단
	else if (abs((vPos.y + (vScale.y / 2.f)) - (vObjPos.y - (vObjScale.y / 2.f))) <= 3.f
		&& vPos.y + (vScale.y / 2.f) <= vObjPos.y + (vObjScale.y / 2.f)
		&& vPos.x - (vScale.x / 2.f) < vObjPos.x + (vObjScale.x / 2.f)
		&& vPos.x + (vScale.x / 2.f) > vObjPos.x - (vObjScale.x / 2.f)
		&& m_bBottom)
	{
		float fLen = abs(vObjPos.y - vPos.y);
		float fValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;

		vObjPos = pOtherObj->GetPos();
		vObjPos.y += fValue;

		pOtherObj->SetPos(vObjPos);
	}
	// 우측
	else if (int(vPos.x + (vScale.x / 2.f)) > int(vObjPos.x - (vObjScale.x / 2.f))
		&& vPos.x + (vScale.x / 2.f) < vObjPos.x + (vObjScale.x / 2.f)
		&& vPos.y - (vScale.y / 2.f) < vObjPos.y + (vObjScale.y / 2.f)
		&& vPos.y + (vScale.y / 2.f) > vObjPos.y - (vObjScale.y / 2.f)
		&& m_bRight)
	{

		float fLen = abs(vObjPos.x - vPos.x);
		float fValue = (vObjScale.x / 2.f + vScale.x / 2.f) - fLen;

		vObjPos = pOtherObj->GetPos();
		vObjPos.x += fValue;

		pRigid->SetVelocity(Vec2(0.f, vCurVelocity.y));
		pOtherObj->SetPos(vObjPos);
	}
	// 좌측
	else if (int(vPos.x - (vScale.x / 2.f)) < int(vObjPos.x + (vObjScale.x / 2.f))
		&& vPos.x - (vScale.x / 2.f) > vObjPos.x - (vObjScale.x / 2.f)
		&& vPos.y - (vScale.y / 2.f) < vObjPos.y + (vObjScale.y / 2.f)
		&& vPos.y + (vScale.y / 2.f) > vObjPos.y - (vObjScale.y / 2.f)
		&& m_bLeft)
	{

		float fLen = abs(vObjPos.x - vPos.x);
		float fValue = (vObjScale.x / 2.f + vScale.x / 2.f) - fLen;

		vObjPos = pOtherObj->GetPos();
		vObjPos.x -= fValue;

		pRigid->SetVelocity(Vec2(0.f, vCurVelocity.y));
		pOtherObj->SetPos(vObjPos);
	}
}

void CFoothold::ChangeBlock()
{
	if (GetTarget())
	{
		if (KEY_TAP(KEY::UP)) m_bTop = !m_bTop;
		if (KEY_TAP(KEY::RIGHT)) m_bRight = !m_bRight;
		if (KEY_TAP(KEY::DOWN)) m_bBottom = !m_bBottom;
		if (KEY_TAP(KEY::LEFT)) m_bLeft = !m_bLeft;
	}
}
