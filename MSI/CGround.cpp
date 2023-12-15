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
	//CreateCollider();
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
	// �⺻������ �ڱ� �ڽŸ�ŭ�� ũ�⸦ �������� ��.
	if (GetComponent()->GetCollider() != nullptr)
	{
		GetComponent()->GetCollider()->SetScale(Vec2(GetScale()));
	}
}

void CGround::update()
{

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

		// �浹 ������ ���� 1�ȼ� �� �÷���.
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
		if (_pOther->GetObj()->GetName().substr(0, 7) == L"Monster")
		{
			// ������ ����
			float fMonPos = _pOther->GetFinalPos().x;
			float fMonHalfSide = _pOther->GetScale().x / 2;

			// ���� ����
			float fGroundPos = GetPos().x;
			float fGroundHalfSide = GetScale().x / 2;

			// ���� �Ÿ��� ��� �������� ��. (�¿�)

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

		// �浹 ������ ���� 1�ȼ� �� �÷���.
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
