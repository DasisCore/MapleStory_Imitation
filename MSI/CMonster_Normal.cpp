#include "global.h"
#include "CMonster_Normal.h"
#include "CTexture.h"
#include "CResMgr.h"
#include "CComponent.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CTimeMgr.h"

CMonster_Normal::CMonster_Normal()
	: CMonster()
	, m_fAttDelay(1.46f)
	, m_fAttTime(1.46f)
{
	CreateGravity();
	CreateRigidbody();

	//CTexture* pLeshLeftTex = CResMgr::GetInst()->LoadTexture(L"Monster_Left_Lesh", L"Texture\\Monster\\Lesh.png");
	//CTexture* pLeshRightTex = CResMgr::GetInst()->LoadTexture(L"Monster_Right_Lesh", L"Texture\\Monster\\Lesh.png", 1);

	//GetComponent()->GetAnimator()->CreateAnimation(L"LESH_LEFT_IDLE", pLeshLeftTex, Vec2(1000.f, 260.f), Vec2(146.f, 100.f), Vec2(146.f, 0.f), 0.1f, 6);
	//GetComponent()->GetAnimator()->CreateAnimation(L"LESH_RIGHT_IDLE", pLeshRightTex, Vec2(30.f, 260.f), Vec2(146.f, 100.f), Vec2(146.f, 0.f), 0.1f, 6);

	//GetComponent()->GetAnimator()->CreateAnimation(L"LESH_LEFT_WALK", pLeshLeftTex, Vec2(130.f, 260.f), Vec2(146.f, 100.f), Vec2(146.f, 0.f), 0.1f, 6);
	//GetComponent()->GetAnimator()->CreateAnimation(L"LESH_RIGHT_WALK", pLeshRightTex, Vec2(1640.f, 260.f), Vec2(146.f, 100.f), Vec2(-146.f, 0.f), 0.1f, 6);

	//GetComponent()->GetAnimator()->CreateAnimation(L"LESH_LEFT_ATTACK", pLeshLeftTex, Vec2(0.f, 0.f), Vec2(146.f, 130.f), Vec2(146.f, 0.f), 0.1f, 15, 13);
	//GetComponent()->GetAnimator()->CreateAnimation(L"LESH_RIGHT_ATTACK", pLeshRightTex, Vec2(1752.f, 0.f), Vec2(146.f, 130.f), Vec2(-146.f, 0.f), 0.1f, 15, -13);

	//GetComponent()->GetAnimator()->CreateAnimation(L"LESH_LEFT_HIT", pLeshLeftTex, Vec2(0.f, 262.f), Vec2(146.f, 131.f), Vec2(146.f, 0.f), 0.1f, 1);
	//GetComponent()->GetAnimator()->CreateAnimation(L"LESH_RIGHT_HIT", pLeshRightTex, Vec2(1752.f, 262.f), Vec2(146.f, 131.f), Vec2(146.f, 0.f), 0.1f, 1);

	//GetComponent()->GetAnimator()->CreateAnimation(L"LESH_LEFT_DIE", pLeshLeftTex, Vec2(292.f, 131.f), Vec2(146.f, 131.f), Vec2(146.f, 0.f), 0.1f, 11);
	//GetComponent()->GetAnimator()->CreateAnimation(L"LESH_RIGHT_DIE", pLeshRightTex, Vec2(1470.f, 131.f), Vec2(146.f, 131.f), Vec2(-146.f, 0.f), 0.1f, 11);
}

CMonster_Normal::CMonster_Normal(wstring _strName, Vec2 _vPos, Vec2 _vScale
	, bool _bCollider, Vec2 _vColOffset, Vec2 _vColScale
	, bool _bAnimation, vector<wstring> _vecPath
	, bool _bGravity, bool _bRigidBody)
	: CMonster()
	, m_fAttDelay(1.46f)
	, m_fAttTime(1.46f)
{
	SetName(_strName);
	SetPos(_vPos);
	SetScale(_vScale);

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
		CAnimator* pAni = GetComponent()->GetAnimator();
		for (int i = 0; i < _vecPath.size(); i++)
		{
			pAni->LoadAnimation(_vecPath[i]);
		}
	}
	if (_bGravity) CreateGravity();
	if (_bRigidBody) CreateRigidbody();
}

CMonster_Normal::~CMonster_Normal()
{
}

void CMonster_Normal::update()
{
	CMonster::update();
}

void CMonster_Normal::Attack()
{
	m_fAttTime -= fDT;
}