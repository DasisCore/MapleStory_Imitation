#include "global.h"
#include "CRaven.h"

#include "CResMgr.h"
#include "CComponent.h"
#include "CAnimator.h"
#include "CCollider.h"


CRaven::CRaven()
{

	//CTexture* m_pLeftTex = CResMgr::GetInst()->LoadTexture(L"RavenLeft", L"Texture\\Player\\Raven.png");
	//CTexture* m_pRightTex = CResMgr::GetInst()->LoadTexture(L"RavenRight", L"Texture\\Player\\Raven.png", 1);

	//// 기본 IDLE 상태는 rewind 필요
	//GetComponent()->GetAnimator()->CreateAnimation_rewind(L"RAVEN_LEFT_IDLE", m_pLeftTex, Vec2(645.f, 75.f), Vec2(73.f, 79.f), Vec2(73.f, 0.f), 0.5f, 3);
	//GetComponent()->GetAnimator()->CreateAnimation_rewind(L"RAVEN_RIGHT_IDLE", m_pRightTex, Vec2(375.f, 75.f), Vec2(73.f, 79.f), Vec2(73.f, 0.f), 0.5f, 3);

	//GetComponent()->GetAnimator()->CreateAnimation(L"RAVEN_LEFT_WALK", m_pLeftTex, Vec2(865.f, 76.f), Vec2(73.f, 78.f), Vec2(73.f, 0.f), 0.1f, 4);
	//GetComponent()->GetAnimator()->CreateAnimation(L"RAVEN_RIGHT_WALK", m_pRightTex, Vec2(85.f, 76.f), Vec2(73.f, 78.f), Vec2(73.f, 0.f), 0.1f, 4);

	//GetComponent()->GetAnimator()->CreateAnimation(L"RAVEN_LEFT_JUMP", m_pLeftTex, Vec2(62.f, 75.f), Vec2(73.f, 79.f), Vec2(0.f, 0.f), 0.1f, 1);
	//GetComponent()->GetAnimator()->CreateAnimation(L"RAVEN_RIGHT_JUMP", m_pRightTex, Vec2(1105.f, 75.f), Vec2(73.f, 79.f), Vec2(0.f, 0.f), 0.1f, 1);

	//GetComponent()->GetAnimator()->CreateAnimation(L"RAVEN_LEFT_PRONE", m_pLeftTex, Vec2(280.f, 75.f), Vec2(85.f, 75.f), Vec2(0.f, 0.f), 1.f, 1);
	//GetComponent()->GetAnimator()->CreateAnimation(L"RAVEN_RIGHT_PRONE", m_pRightTex, Vec2(730.f, 76.f), Vec2(73.f, 75.f), Vec2(0.f, 0.f), 1.f, 1);

}

CRaven::~CRaven()
{
}


void CRaven::update_move()
{
	CPlayer::update_move();
}
