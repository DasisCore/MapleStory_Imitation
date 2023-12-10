#include "global.h"
#include "CLesh.h"
#include "CTexture.h"
#include "CResMgr.h"
#include "CComponent.h"
#include "CAnimator.h"
#include "CTimeMgr.h"

CLesh::CLesh()
	: CMonster()
	, m_fAttDelay(1.5f)
	, m_fAttTime(1.5f)
{
	setCurChar(L"LESH");

	CreateGravity();

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

CLesh::~CLesh()
{
}

void CLesh::update()
{
	CMonster::update();
	//GetComponent()->GetAnimator()->Play(L"LESH_RIGHT_WALK", true);
}

void CLesh::Attack()
{
	m_fAttTime -= fDT;
	//GetComponent()->GetAnimator()->Play(L"LESH_RIGHT_ATTACK", true);
}