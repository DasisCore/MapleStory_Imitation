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
#include "CDamegeFactory.h"
#include "CDamege.h"
#include "CSoundMgr.h"
#include "CEffect.h"
#include "CCore.h"
#include "CRandom.h"

CPlayer::CPlayer()
	: m_wCurChar(L"RAVEN")
	, m_eCurState(PLAYER_STATE::JUMP)
	, m_ePrevState(PLAYER_STATE::WALK)
	, m_iDir(1)
	, m_iPrevDir(1)
	, m_bIsGround(0)
	, m_bIsAir(0)
	, m_tPlayerInfo{ 37, 1500, 1500, 100, 100, 1000, 30 }
	, m_bUseDoubleJump(false)
{
	CreateComponent();

	// 캐릭터 기본 콜라이더 크기 (45, 70)
	CreateCollider();
	GetComponent()->GetCollider()->SetScale(Vec2(45.f, 70.f));
	GetComponent()->GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));

	CreateRigidbody();

	CreateAnimator();

	GetComponent()->GetAnimator()->LoadAnimation(L"Animation\\RAVEN_LEFT_IDLE.anim");
	GetComponent()->GetAnimator()->LoadAnimation(L"Animation\\RAVEN_RIGHT_IDLE.anim");

	GetComponent()->GetAnimator()->LoadAnimation(L"Animation\\RAVEN_LEFT_WALK.anim");
	GetComponent()->GetAnimator()->LoadAnimation(L"Animation\\RAVEN_RIGHT_WALK.anim");

	GetComponent()->GetAnimator()->LoadAnimation(L"Animation\\RAVEN_LEFT_JUMP.anim");
	GetComponent()->GetAnimator()->LoadAnimation(L"Animation\\RAVEN_RIGHT_JUMP.anim");

	GetComponent()->GetAnimator()->LoadAnimation(L"Animation\\RAVEN_LEFT_ATT.anim");
	GetComponent()->GetAnimator()->LoadAnimation(L"Animation\\RAVEN_RIGHT_ATT.anim");

	// =====================================
	// 중력 추가
	// =====================================
	CreateGravity();

	CSoundMgr::GetInst()->CreateSound(L"jump", "jump.mp3", false);
	CSoundMgr::GetInst()->CreateSound(L"doublejump", "doublejump.mp3", false);
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

	update_status();

	if (KEY_TAP(KEY::O))
	{
		CharHit(-1, 10);
	}
	if (KEY_TAP(KEY::P))
	{
		CharHit(1, 10);
	}

	if (m_fUnbeatableTime >= 0) m_fUnbeatableTime -= fDT;
	if (m_bIsGround) m_bUseDoubleJump = false;

	Delay();
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
		// 땅에 붙어있었다면 일반 점프
		if (m_bIsGround)
		{
			m_eCurState = PLAYER_STATE::JUMP;
			m_bIsAir = 1;
			m_bIsGround = 0;
			GetComponent()->GetRigidbody()->SetVelocity(Vec2(GetComponent()->GetRigidbody()->GetVelocity().x, -640.f));
			CSoundMgr::GetInst()->Play(L"jump", SOUND_TYPE::EFFECT1);
		}
		else if (m_bUseDoubleJump == false)
		{
			if (KEY_HOLD(KEY::RIGHT)) GetComponent()->GetRigidbody()->SetVelocity(Vec2(+500.f, -240.f));
			else if (KEY_HOLD(KEY::LEFT)) GetComponent()->GetRigidbody()->SetVelocity(Vec2(-500.f, -240.f));
			else if (KEY_HOLD(KEY::UP)) GetComponent()->GetRigidbody()->SetVelocity(Vec2(GetComponent()->GetRigidbody()->GetVelocity().x, -840.f));

			
			if (KEY_HOLD(KEY::RIGHT) || KEY_HOLD(KEY::LEFT))
			{
				CSoundMgr::GetInst()->Play(L"doublejump", SOUND_TYPE::EFFECT1);
			
				CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
				CEffect* pEffect = new CEffect(this, L"DoubleJump", 10, 0.05f, m_iDir * -1, Vec2(-40.f, -65.f));
				pCurScene->AddObject(pEffect, GROUP_TYPE::EFFECT);
			
				m_bUseDoubleJump = true;
			}
			else if (KEY_HOLD(KEY::UP))
			{
				CSoundMgr::GetInst()->Play(L"doublejump", SOUND_TYPE::EFFECT1);

				CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
				CEffect* pEffect = new CEffect(this, L"UpJump", 11, 0.05f, -1, Vec2(45.f, 0.f));
				pCurScene->AddObject(pEffect, GROUP_TYPE::EFFECT);

				m_bUseDoubleJump = true;
			}
		}
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
	if (m_fUnbeatableTime >= 0)
	{
		float temp = fDT;
		m_fBlinkTime -= fDT;

		if (m_fBlinkTime > 0)
		{
			GetComponent()->GetAnimator()->Play(L"VOID", true);
			return;
		}
		else
		{
			m_fBlinkTime = 0.03f;
		}
	}

	//if (m_ePrevState == m_eCurState && m_iPrevDir == m_iDir && m_fUnbeatableTime <= 0) return;

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

void CPlayer::update_status()
{
	if (m_tPlayerInfo.iHP < 0) m_tPlayerInfo.iHP = 0;
	if (m_tPlayerInfo.iMP < 0) m_tPlayerInfo.iMP = 0;
	if (m_tPlayerInfo.iExp < 0) m_tPlayerInfo.iExp = 0;

	if (m_tPlayerInfo.iHP > m_tPlayerInfo.iMaxHP) m_tPlayerInfo.iHP = m_tPlayerInfo.iMaxHP;
	if (m_tPlayerInfo.iMP > m_tPlayerInfo.iMaxMP) m_tPlayerInfo.iMP = m_tPlayerInfo.iMaxMP;
	if (m_tPlayerInfo.iExp > m_tPlayerInfo.iMaxExp) m_tPlayerInfo.iExp = m_tPlayerInfo.iMaxExp;
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
}

void CPlayer::show_state(HDC _dc)
{
	if (CCore::GetInst()->GetRenderOption())
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

		_iDamege = CRandom::GetInst()->GetBetweenInt(int(_iDamege * 0.8), int(_iDamege * 1.2));
		m_tPlayerInfo.iHP -= _iDamege;

		CDamegeFactory::CreateSingleDamege(this, _iDamege, 0, DAMEGE_TYPE::VIOLET);
	}
}
