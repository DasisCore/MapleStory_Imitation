#include "global.h"
#include "CBackground.h"

#include "CComponent.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CCollider.h"

CBackground::CBackground()
{

}

CBackground::CBackground(wstring _strName, Vec2 _vPos, Vec2 _vScale, bool _bCollider, Vec2 _vColOffset, Vec2 _vColScale, bool _bAnimation, vector<wstring> _vecPath, bool _bGravity, bool _bRigidBody)
{
	SetName(_strName);
	SetPos(_vPos);
	SetScale(_vScale);

	CreateComponent();

	// 애니메이션 생성
	tAnimFrm Frm;
	if (_bAnimation)
	{
		CreateAnimator();
		CAnimator* pAnimator = GetComponent()->GetAnimator();
		for (int i = 0; i < _vecPath.size(); i++) pAnimator->LoadAnimation(_vecPath[i]);
		pAnimator->Play(_strName, 1);

		if (_vScale == Vec2(0.f, 0.f))
		{
			CAnimation* pAnimation = pAnimator->FindAnimation(_strName);
			Frm = pAnimation->GetFrame(0);
			SetScale(Frm.vSlice);
		}
	}

	if (_bCollider)
	{
		CreateCollider();
		CCollider* pCol = GetComponent()->GetCollider();
		pCol->SetOffsetPos(_vColOffset);
		pCol->SetScale(_vColScale);
		
		// 만약 콜라이더 크기가 Vec2(0.f, 0.f)라면 애니메이션 크기로 설정한다.
		if (_vColScale == Vec2(0.f, 0.f)) pCol->SetScale(Frm.vSlice);	
	}

	if (_bGravity) CreateGravity();
	if (_bRigidBody) CreateRigidbody();
}

CBackground::~CBackground()
{


}

void CBackground::update()
{


}
