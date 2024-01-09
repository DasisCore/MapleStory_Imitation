#include "global.h"
#include "CPlayer.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"

#include "CSceneMgr.h"
#include "CScene.h"

#include "CMissile.h"

#include "CResMgr.h"
#include "CPathMgr.h"
#include "CTexture.h"

#include "CComponent.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CRigidBody.h"
#include "CGravity.h"

CPlayer::CPlayer()
	: m_wCurChar(L"RAVEN")
	, m_eCurState(PLAYER_STATE::JUMP)
	, m_ePrevState(PLAYER_STATE::WALK)
	, m_iDir(1)
	, m_iPrevDir(1)
	, m_bIsGround(0)
	, m_bIsAir(0)
	, m_tPlayerInfo{ 37, 1367, 857, 100, 20, 3300, 30 }
{
	CreateComponent();

	// 캐릭터 기본 콜라이더 크기 (45, 70)
	CreateCollider();
	GetComponent()->GetCollider()->SetScale(Vec2(45.f, 70.f));
	GetComponent()->GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));


	CreateRigidbody();
	

	//CTexture* pTex = CResMgr::GetInst()->LoadTexture(L"PlayerTex", L"Texture\\link_0.bmp");
	//CTexture* pLeftTex = CResMgr::GetInst()->LoadTexture(L"PlayerLeft", L"Texture\\player01_L.bmp");
	//CTexture* pRightTex = CResMgr::GetInst()->LoadTexture(L"PlayerRight", L"Texture\\player01_R.bmp");

	CreateAnimator();

	GetComponent()->GetAnimator()->LoadAnimation(L"Animation\\RAVEN_LEFT_IDLE.anim");
	GetComponent()->GetAnimator()->LoadAnimation(L"Animation\\RAVEN_RIGHT_IDLE.anim");

	GetComponent()->GetAnimator()->LoadAnimation(L"Animation\\RAVEN_LEFT_WALK.anim");
	GetComponent()->GetAnimator()->LoadAnimation(L"Animation\\RAVEN_RIGHT_WALK.anim");

	GetComponent()->GetAnimator()->LoadAnimation(L"Animation\\RAVEN_LEFT_JUMP.anim");
	GetComponent()->GetAnimator()->LoadAnimation(L"Animation\\RAVEN_RIGHT_JUMP.anim");

	GetComponent()->GetAnimator()->LoadAnimation(L"Animation\\RAVEN_LEFT_ATT.anim");
	GetComponent()->GetAnimator()->LoadAnimation(L"Animation\\RAVEN_RIGHT_ATT.anim");

	//////CTexture* m_pLeftTex = CResMgr::GetInst()->LoadTexture(L"RavenLeft", L"Texture\\Player\\Raven.png");
	//////CTexture* m_pRightTex = CResMgr::GetInst()->LoadTexture(L"RavenRight", L"Texture\\Player\\Raven.png", 1);

	//////// 기본 IDLE 상태는 rewind 필요
	//////GetComponent()->GetAnimator()->CreateAnimation_rewind(L"RAVEN_LEFT_IDLE", m_pLeftTex, Vec2(645.f, 75.f), Vec2(73.f, 79.f), Vec2(73.f, 0.f), 0.5f, 3);
	//////GetComponent()->GetAnimator()->CreateAnimation_rewind(L"RAVEN_RIGHT_IDLE", m_pRightTex, Vec2(375.f, 75.f), Vec2(73.f, 79.f), Vec2(73.f, 0.f), 0.5f, 3);

	//////GetComponent()->GetAnimator()->CreateAnimation(L"RAVEN_LEFT_WALK", m_pLeftTex, Vec2(865.f, 76.f), Vec2(73.f, 78.f), Vec2(73.f, 0.f), 0.1f, 4);
	//////GetComponent()->GetAnimator()->CreateAnimation(L"RAVEN_RIGHT_WALK", m_pRightTex, Vec2(85.f, 76.f), Vec2(73.f, 78.f), Vec2(73.f, 0.f), 0.1f, 4);

	//////GetComponent()->GetAnimator()->CreateAnimation(L"RAVEN_LEFT_JUMP", m_pLeftTex, Vec2(62.f, 75.f), Vec2(73.f, 79.f), Vec2(0.f, 0.f), 0.1f, 1);
	//////GetComponent()->GetAnimator()->CreateAnimation(L"RAVEN_RIGHT_JUMP", m_pRightTex, Vec2(1105.f, 75.f), Vec2(73.f, 79.f), Vec2(0.f, 0.f), 0.1f, 1);

	//////GetComponent()->GetAnimator()->CreateAnimation(L"RAVEN_LEFT_PRONE", m_pLeftTex, Vec2(280.f, 75.f), Vec2(85.f, 75.f), Vec2(0.f, 0.f), 1.f, 1);
	//////GetComponent()->GetAnimator()->CreateAnimation(L"RAVEN_RIGHT_PRONE", m_pRightTex, Vec2(730.f, 76.f), Vec2(73.f, 75.f), Vec2(0.f, 0.f), 1.f, 1);


	////// Animation 저장해보기
	////GetComponent()->GetAnimator()->FindAnimation(L"IDLE_LEFT")->Save(L"\\Animation\\player_idle_left.anim");
	////GetComponent()->GetAnimator()->FindAnimation(L"IDLE_RIGHT")->Save(L"\\Animation\\player_idle_right.anim");

	////GetComponent()->GetAnimator()->FindAnimation(L"WALK_LEFT")->Save(L"\\Animation\\player_walk_left.anim");
	////GetComponent()->GetAnimator()->FindAnimation(L"WALK_RIGHT")->Save(L"\\Animation\\player_walk_right.anim");

	////GetComponent()->GetAnimator()->FindAnimation(L"JUMP_LEFT")->Save(L"\\Animation\\player_jump_left.anim");
	////GetComponent()->GetAnimator()->FindAnimation(L"JUMP_RIGHT")->Save(L"\\Animation\\player_jump_right.anim");

	//CAnimation* pAnim = GetComponent()->GetAnimator()->FindAnimation(L"WALK_DOWN");
	//pAnim->GetFrame(0).vOffset = Vec2(0.f, -20.f);

	// =====================================
	// 중력 추가
	// =====================================
	CreateGravity();
}

CPlayer::~CPlayer()
{
}

void CPlayer::update()
{
	if (!m_bDelay)
	{
		update_move();

		update_state();

		update_animation();

		GetComponent()->GetAnimator()->update();
		m_ePrevState = m_eCurState;
		m_iPrevDir = m_iDir;
	}

	//if (KEY_TAP(KEY::ENTER))
	//{
	//	SetPos(Vec2(640.f, 384.f));
	//	GetComponent()->GetRigidbody()->SetVelocity(Vec2(0.f, 0.f));
	//}

	if (KEY_TAP(KEY::O))
	{
		CharHit(-1, 10);
	}
	if (KEY_TAP(KEY::P))
	{
		CharHit(1, 10);
	}

	if (m_fUnbeatableTime >= 0) m_fUnbeatableTime -= fDT;

	Delay();
}

void CPlayer::CreateMissile()
{
	Vec2 vMissilePos = GetPos();

	vMissilePos.y -= GetScale().y / 2.f;

	CMissile* pMissile = new CMissile;
	pMissile->SetPos(vMissilePos);
	pMissile->SetScale(Vec2(25.f, 25.f));

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	pCurScene->AddObject(pMissile, GROUP_TYPE::MISSILE);
}

void CPlayer::update_state()
{
	if (KEY_HOLD(KEY::LEFT))
	{
		m_iDir = -1;
		if (m_eCurState == PLAYER_STATE::IDLE && m_bIsGround)
		{
			m_eCurState = PLAYER_STATE::WALK;
		}
	}

	if (KEY_HOLD(KEY::RIGHT))
	{
		m_iDir = 1;
		if (m_eCurState == PLAYER_STATE::IDLE && m_bIsGround)
		{
			m_eCurState = PLAYER_STATE::WALK;
		}
	}

	if (KEY_AWAY(KEY::LEFT))
	{
		m_iDir = -1;
		if (m_eCurState != PLAYER_STATE::JUMP && m_bIsGround)
		{
			m_eCurState = PLAYER_STATE::IDLE;
		}
	}

	if (KEY_AWAY(KEY::RIGHT))
	{
		m_iDir = 1;
		if (m_eCurState != PLAYER_STATE::JUMP && m_bIsGround)
		{
			m_eCurState = PLAYER_STATE::IDLE;
		}
	}

	if (m_bIsGround && KEY_HOLD(KEY::DOWN))
	{
		m_eCurState = PLAYER_STATE::PRONE;
	}

	if (m_bIsGround && KEY_AWAY(KEY::DOWN))
	{
		m_eCurState = PLAYER_STATE::IDLE;
	}


	if (0.f == GetComponent()->GetRigidbody()->GetSpeed() 
		&& m_eCurState != PLAYER_STATE::JUMP 
		&& m_eCurState != PLAYER_STATE::ATTACK 
		&& m_bIsGround)
	{
		m_eCurState = PLAYER_STATE::IDLE;
	}

	if (KEY_TAP(KEY::ALT))
	{
		m_eCurState = PLAYER_STATE::JUMP;
		m_bIsAir = 1;
		m_bIsGround = 0;
		GetComponent()->GetRigidbody()->SetVelocity(Vec2(GetComponent()->GetRigidbody()->GetVelocity().x, -740.f));
	}
}

void CPlayer::update_move()
{

	CRigidBody* pRigid = GetComponent()->GetRigidbody();

	if (KEY_HOLD(KEY::LEFT))
	{
		if (m_bIsGround)
		{
			pRigid->SetVelocity(Vec2(-300.f, pRigid->GetVelocity().y));
			pRigid->AddForce(Vec2(-900.f, 0.f));
		}
		else 
		{
			pRigid->AddForce(Vec2(-100.f, 0.f));
		}

	}

	if (KEY_HOLD(KEY::RIGHT))
	{
		if (m_bIsGround)
		{
			pRigid->SetVelocity(Vec2(300.f, pRigid->GetVelocity().y));
			pRigid->AddForce(Vec2(900.f, 0.f));
		}
		else
		{
			pRigid->AddForce(Vec2(100.f, 0.f));
		}
	}


	if (KEY_TAP(KEY::LEFT) && m_bIsGround)
	{
		pRigid->SetVelocity(Vec2(-300.f, pRigid->GetVelocity().y));
	}
	if (KEY_TAP(KEY::RIGHT) && m_bIsGround)
	{
		pRigid->SetVelocity(Vec2(300.f, pRigid->GetVelocity().y));
	}

}

void CPlayer::update_animation()
{
	if (m_ePrevState == m_eCurState && m_iPrevDir == m_iDir) return;

	wstring currentChar = m_wCurChar;

	if (m_iDir == -1) currentChar += L"_LEFT";
	else currentChar += L"_RIGHT";

	switch (m_eCurState)
	{
	case PLAYER_STATE::IDLE:
	{
		currentChar += L"_IDLE";
		GetComponent()->GetAnimator()->Play(currentChar.c_str(), true);
	}
		break;

	case PLAYER_STATE::WALK:
	{
		currentChar += L"_WALK";
		GetComponent()->GetAnimator()->Play(currentChar.c_str(), true);
	}
		break;

	case PLAYER_STATE::ALERT:
		break;

	case PLAYER_STATE::ATTACK:
	{
		currentChar += L"_ATT";
		GetComponent()->GetAnimator()->Play(currentChar.c_str(), false);
		GetComponent()->GetAnimator()->FindAnimation(currentChar.c_str())->SetFrame(0);
	}
		break;
	case PLAYER_STATE::JUMP:
	{
		currentChar += L"_JUMP";
		GetComponent()->GetAnimator()->Play(currentChar.c_str(), true);
	}
		break;
	case PLAYER_STATE::PRONE:
	{		
		currentChar += L"_PRONE";
		GetComponent()->GetAnimator()->Play(currentChar.c_str(), true);
	}
		break;
	case PLAYER_STATE::DEAD:
		break;
	}
}


void CPlayer::OnCollisionEnter(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Ground" || pOtherObj->GetName() == L"Foothold")
	{
		Vec2 vPos = GetPos();
		if (vPos.y < pOtherObj->GetPos().y)
		{
			m_eCurState = PLAYER_STATE::IDLE;
			m_bIsGround = 1;
			m_bIsAir = 0;
		}
	}


}

void CPlayer::OnCollision(CCollider* _pOther)
{
	if (m_fUnbeatableTime < 0)
	{
		CObject* pOtherObj = _pOther->GetObj();
		if (pOtherObj->GetName() == L"LESH")
		{
			Vec2 otherObjvPos = pOtherObj->GetPos();
			Vec2 vPos = GetPos();

			//if (otherObjvPos.x - vPos.x <= 0)
			//{
			//	CharHit(1);
			//}
			//else
			//{
			//	CharHit(-1);
			//}
		}
	}
}

void CPlayer::Delay()
{
	if (m_fDelayTime > 0)
	{
		m_fDelayTime -= fDT;
		m_bDelay = true;
		m_bCanOtherAction = false;
		return;
	}

	if (m_eCurState == PLAYER_STATE::ATTACK) m_eCurState = PLAYER_STATE::IDLE;
	
	m_bCanOtherAction = true;
	m_bDelay = false;
}

void CPlayer::render(HDC _dc)
{
	// 컴포넌트 충돌체, 애니메이션등 렌더링
	component_render(_dc);
	show_state(_dc);

	Graphics g(_dc);
	Pen pen(Color(0, 0, 255));
	g.DrawLine(&pen, Point(-1000, GetPos().y + GetScale().y / 2.f), Point(+3000, GetPos().y + GetScale().y / 2.f));
	g.DrawLine(&pen, Point(GetPos().x - GetScale().x / 2.f, -1000), Point(GetPos().x - GetScale().x / 2.f, 3000));
}

void CPlayer::show_state(HDC _dc)
{
	Graphics graphics(_dc);

	Font font(L"Arial", 13, FontStyle::FontStyleBold);
	SolidBrush brush(Color(255, 0, 0, 0));

	Vec2 vPos = GetPos();
	vPos = CCamera::GetInst()->GetRenderPos(vPos);

	wstring current_state = L"";

	if (m_eCurState == PLAYER_STATE::IDLE)
	{
		current_state = L"IDLE";
	}
	else if (m_eCurState == PLAYER_STATE::ATTACK)
	{
		current_state = L"ATT";
	}
	else if (m_eCurState == PLAYER_STATE::DEAD)
	{
		current_state = L"DEAD";
	}
	else if (m_eCurState == PLAYER_STATE::JUMP)
	{
		current_state = L"JUMP";
	}
	else if (m_eCurState == PLAYER_STATE::PRONE)
	{
		current_state = L"PRONE";
	}
	else if (m_eCurState == PLAYER_STATE::PRONE_ATT)
	{
		current_state = L"PRONE_ATT";
	}
	else if (m_eCurState == PLAYER_STATE::WALK)
	{
		current_state = L"WALK";
	}

	if (m_iDir == -1)
	{
		current_state += L"_LEFT";
	}
	else if (m_iDir == 1)
	{
		current_state += L"_RIGHT";
	}

	PointF point(vPos.x + 30.f, vPos.y);

	graphics.DrawString(GetName().c_str(), -1, &font, PointF(vPos.x - 22.f, vPos.y - 55.f), &brush);

	Font font2(L"Arial", 10);
	graphics.DrawString(current_state.c_str(), -1, &font2, point, &brush);
}

void CPlayer::CharHit(int _iDir, int _iDamege)
{
	if (m_fUnbeatableTime < 0)
	{
		m_fUnbeatableTime = 1.f;

		CRigidBody* pRigid = GetComponent()->GetRigidbody();

		if (_iDir == -1)
		{
			pRigid->SetVelocity(Vec2(-250.f, -250.f));
		}
		else if(_iDir == 1)
		{
			pRigid->SetVelocity(Vec2(+250.f, -250.f));
		}


		m_tPlayerInfo.iHP -= _iDamege;
	}
}
