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
	, m_eCurState(PLAYER_STATE::IDLE)
	, m_ePrevState(PLAYER_STATE::WALK)
	, m_iDir(1)
	, m_iPrevDir(1)
	, m_bIsGround(0)
	, m_bIsAir(0)
{
	// 임시 텍스쳐 로딩
	//m_pTex = new CTexture;

	//wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	//strFilePath += L"texture\\player.bmp";
	//m_pTex->Load(strFilePath);

	// 위 작업을 CResMgr를 이용

	CreateComponent();

	// 캐릭터 기본 콜라이더 크기 (45, 70)
	CreateCollider();
	GetComponent()->GetCollider()->SetScale(Vec2(45.f, 70.f));
	GetComponent()->GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));


	CreateRigidbody();
	

	//CTexture* pTex = CResMgr::GetInst()->LoadTexture(L"PlayerTex", L"Texture\\link_0.bmp");
	//CTexture* pLeftTex = CResMgr::GetInst()->LoadTexture(L"PlayerLeft", L"Texture\\player01_L.bmp");
	//CTexture* pRightTex = CResMgr::GetInst()->LoadTexture(L"PlayerRight", L"Texture\\player01_R.bmp");

	CreateAnimation();

	//GetComponent()->GetAnimator()->LoadAnimation(L"\\Animation\\player_idle_left.anim");
	//GetComponent()->GetAnimator()->LoadAnimation(L"\\Animation\\player_idle_right.anim");

	//GetComponent()->GetAnimator()->LoadAnimation(L"\\Animation\\player_walk_left.anim");
	//GetComponent()->GetAnimator()->LoadAnimation(L"\\Animation\\player_walk_right.anim");

	//GetComponent()->GetAnimator()->LoadAnimation(L"\\Animation\\player_jump_left.anim");
	//GetComponent()->GetAnimator()->LoadAnimation(L"\\Animation\\player_jump_right.anim");

	CTexture* m_pLeftTex = CResMgr::GetInst()->LoadTexture(L"RavenLeft", L"Texture\\Player\\Raven.png");
	CTexture* m_pRightTex = CResMgr::GetInst()->LoadTexture(L"RavenRight", L"Texture\\Player\\Raven.png", 1);

	// 기본 IDLE 상태는 rewind 필요
	GetComponent()->GetAnimator()->CreateAnimation_rewind(L"RAVEN_LEFT_IDLE", m_pLeftTex, Vec2(645.f, 75.f), Vec2(73.f, 79.f), Vec2(73.f, 0.f), 0.5f, 3);
	GetComponent()->GetAnimator()->CreateAnimation_rewind(L"RAVEN_RIGHT_IDLE", m_pRightTex, Vec2(375.f, 75.f), Vec2(73.f, 79.f), Vec2(73.f, 0.f), 0.5f, 3);

	GetComponent()->GetAnimator()->CreateAnimation(L"RAVEN_LEFT_WALK", m_pLeftTex, Vec2(865.f, 76.f), Vec2(73.f, 78.f), Vec2(73.f, 0.f), 0.1f, 4);
	GetComponent()->GetAnimator()->CreateAnimation(L"RAVEN_RIGHT_WALK", m_pRightTex, Vec2(85.f, 76.f), Vec2(73.f, 78.f), Vec2(73.f, 0.f), 0.1f, 4);

	GetComponent()->GetAnimator()->CreateAnimation(L"RAVEN_LEFT_JUMP", m_pLeftTex, Vec2(62.f, 75.f), Vec2(73.f, 79.f), Vec2(0.f, 0.f), 0.1f, 1);
	GetComponent()->GetAnimator()->CreateAnimation(L"RAVEN_RIGHT_JUMP", m_pRightTex, Vec2(1105.f, 75.f), Vec2(73.f, 79.f), Vec2(0.f, 0.f), 0.1f, 1);

	GetComponent()->GetAnimator()->CreateAnimation(L"RAVEN_LEFT_PRONE", m_pLeftTex, Vec2(280.f, 75.f), Vec2(85.f, 75.f), Vec2(0.f, 0.f), 1.f, 1);
	GetComponent()->GetAnimator()->CreateAnimation(L"RAVEN_RIGHT_PRONE", m_pRightTex, Vec2(730.f, 76.f), Vec2(73.f, 75.f), Vec2(0.f, 0.f), 1.f, 1);


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
	update_move();

	update_state();

	update_animation();

	GetComponent()->GetAnimator()->update();
	m_ePrevState = m_eCurState;
	m_iPrevDir = m_iDir;

	if (KEY_TAP(KEY::ENTER))
	{
		SetPos(Vec2(640.f, 384.f));
		GetComponent()->GetRigidbody()->SetVelocity(Vec2(0.f, 0.f));
	}
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
		if (m_eCurState != PLAYER_STATE::JUMP && m_bIsGround)
		{
			m_eCurState = PLAYER_STATE::WALK;
		}
	}

	if (KEY_HOLD(KEY::RIGHT))
	{
		m_iDir = 1;
		if (m_eCurState != PLAYER_STATE::JUMP && m_bIsGround)
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


	if (0.f == GetComponent()->GetRigidbody()->GetSpeed() && m_eCurState != PLAYER_STATE::JUMP && m_bIsGround)
	{
		m_eCurState = PLAYER_STATE::IDLE;
	}

	if (KEY_TAP(KEY::ALT))
	{
		m_eCurState = PLAYER_STATE::JUMP;
		m_bIsAir = 1;
		m_bIsGround = 0;
		GetComponent()->GetRigidbody()->SetVelocity(Vec2(GetComponent()->GetRigidbody()->GetVelocity().x, -400.f));
	}
}

void CPlayer::update_move()
{
	CRigidBody* pRigid = GetComponent()->GetRigidbody();

	if (KEY_HOLD(KEY::LEFT))
	{
		pRigid->AddForce(Vec2(-200.f, 0.f));
	}

	if (KEY_HOLD(KEY::RIGHT))
	{
		pRigid->AddForce(Vec2(200.f, 0.f));
	}


	if (KEY_TAP(KEY::LEFT))
	{
		pRigid->SetVelocity(Vec2(-100.f, pRigid->GetVelocity().y));
	}
	if (KEY_TAP(KEY::RIGHT))
	{
		pRigid->SetVelocity(Vec2(100.f, pRigid->GetVelocity().y));
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
	if (pOtherObj->GetName() == L"Ground")
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

void CPlayer::render(HDC _dc)
{
	// 컴포넌트 충돌체, 애니메이션등 렌더링
	component_render(_dc);
	show_state(_dc);
}

void CPlayer::show_state(HDC _dc)
{
	Graphics graphics(_dc);

	Font font(L"Arial", 9, FontStyle::FontStyleBold);
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
		current_state = L"ATTACK";
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

	PointF point(vPos.x - 30.f, vPos.y - 55.f);

	graphics.DrawString(current_state.c_str(), -1, &font, point, &brush);
}
