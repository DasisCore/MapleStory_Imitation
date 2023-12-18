#include "global.h"
#include "CGround.h"

#include "CComponent.h"
#include "CCollider.h"
#include "CGravity.h"
#include "CMonster.h"
#include "CAnimator.h"

CGround::CGround()
{
	CreateComponent();
	CreateCollider();
}

CGround::CGround(wstring _strName, Vec2 _vPos, Vec2 _vScale, bool _bCollider, Vec2 _vColOffset, Vec2 _vColScale, bool _bAnimation, vector<wstring> _vecPath, bool _bGravity, bool _bRigidBody)
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

CGround::~CGround()
{
}

void CGround::start()
{
	// 기본적으로 자기 자신만큼의 크기를 가지도록 함.
	if (GetComponent()->GetCollider() != nullptr)
	{
		GetComponent()->GetCollider()->SetScale(Vec2(GetScale()));
	}
}

void CGround::update()
{

}

void CGround::render(HDC _dc)
{
	Graphics graphics(_dc);

	Vec2 vPos = GetPos();
	vPos = CCamera::GetInst()->GetRenderPos(vPos);
	Vec2 vScale = GetScale();

	Rect rectangle(vPos.x - (vScale.x / 2.f), vPos.y - (vScale.y / 2.f), vScale.x, vScale.y);

	PointF startPoint(rectangle.X, rectangle.Y);
	PointF endPoint(rectangle.GetRight(), rectangle.Y);
	
	// 윗변 그리기
	Pen pen1(Color(0, 0, 0), 3);
	graphics.DrawLine(&pen1, startPoint, endPoint);

	// 윗변 제외한 나머지 변 그리기
	Pen pen2(Color(240, 240, 240));
	PointF leftBottom(rectangle.X, rectangle.GetBottom());
	PointF rightBottom(rectangle.GetRight(), rectangle.GetBottom());
	PointF rightTop(rectangle.GetRight(), rectangle.Y + rectangle.Height);
	graphics.DrawLine(&pen2, endPoint, rightBottom);
	graphics.DrawLine(&pen2, rightBottom, leftBottom);
	graphics.DrawLine(&pen2, leftBottom, startPoint);

	Font font(L"Arial", 8);
	SolidBrush brush(Color(0, 0, 0));
	graphics.DrawString(GetName().c_str(), -1, &font, PointF(vPos.x, vPos.y), &brush);
}

void CGround::OnCollisionEnter(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetGroundCheck())
	{
		if (pOtherObj->GetComponent()->GetGravity() != nullptr)
		{
			pOtherObj->GetComponent()->GetGravity()->SetGround(true);
		}

		Vec2 vObjPos = _pOther->GetFinalPos();
		Vec2 vObjScale = _pOther->GetScale();

		Vec2 vPos = GetComponent()->GetCollider()->GetFinalPos();
		Vec2 vScale = GetComponent()->GetCollider()->GetScale();

		float fLen = abs(vObjPos.y - vPos.y);
		float fValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;

		// 충돌 유지를 위해 1픽셀 덜 올려줌.
		vObjPos = pOtherObj->GetPos();
		vObjPos.y -= fValue;

		pOtherObj->SetPos(vObjPos);
	}
}

void CGround::OnCollision(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetGroundCheck())
	{
		//if (_pOther->GetObj()->GetName().substr(0, 7) == L"Monster")
		if(1)
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

		pOtherObj->GetComponent()->GetGravity()->SetGround(true);

		Vec2 vObjPos = _pOther->GetFinalPos();
		Vec2 vObjScale = _pOther->GetScale();

		Vec2 vPos = GetComponent()->GetCollider()->GetFinalPos();
		Vec2 vScale = GetComponent()->GetCollider()->GetScale();

		float fLen = abs(vObjPos.y - vPos.y);
		float fValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;

		// 충돌 유지를 위해 1픽셀 덜 올려줌.
		vObjPos = pOtherObj->GetPos();
		vObjPos.y -= fValue;
		
		pOtherObj->SetPos(vObjPos);
	}
}

void CGround::OnCollisionExit(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetGroundCheck())
	{
		pOtherObj->GetComponent()->GetGravity()->SetGround(false);
	}
}
