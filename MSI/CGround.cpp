#include "global.h"
#include "CGround.h"

#include "CComponent.h"
#include "CCollider.h"
#include "CGravity.h"
#include "CMonster.h"

CGround::CGround()
{
	CreateComponent();
	CreateCollider();
}

CGround::~CGround()
{
}

void CGround::start()
{
	GetComponent()->GetCollider()->SetScale(Vec2(GetScale()));
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
