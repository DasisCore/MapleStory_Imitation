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
	: m_eCurState(PLAYER_STATE::IDLE)
	, m_ePrevState(PLAYER_STATE::WALK)
	, m_iDir(1)
	, m_iPrevDir(1)
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

	//CTexture* m_pLeftTex = CResMgr::GetInst()->LoadTexture(L"PlayerLeft", L"Texture\\Player01_L.bmp");
	CTexture* m_pLeftTex = CResMgr::GetInst()->LoadTexture(L"PlayerLeft", L"Texture\\Agnes.png");
	CTexture* m_pRightTex = CResMgr::GetInst()->LoadTexture(L"PlayerRight", L"Texture\\Player01_R.bmp");

	//GetComponent()->GetAnimator()->CreateAnimation(L"IDLE_LEFT", m_pLeftTex, Vec2(0.f, 6.f), Vec2(26.f, 28.f), Vec2(31.f, 0.f), 0.1f, 8);
	GetComponent()->GetAnimator()->CreateAnimation(L"IDLE_LEFT", m_pLeftTex, Vec2(0.f, 0.f), Vec2(87.f, 90.f), Vec2(0.f, 0.f), 1.f, 1);
	GetComponent()->GetAnimator()->CreateAnimation(L"IDLE_RIGHT", m_pRightTex, Vec2(161.f, 6.f), Vec2(26.f, 28.f), Vec2(31.f, 0.f), 0.1f, 8);

	GetComponent()->GetAnimator()->CreateAnimation(L"WALK_LEFT", m_pLeftTex, Vec2(0.f, 68.f), Vec2(26.f, 28.f), Vec2(31.f, 0.f), 0.1f, 13);
	GetComponent()->GetAnimator()->CreateAnimation(L"WALK_RIGHT", m_pRightTex, Vec2(6.f, 68.f), Vec2(26.f, 28.f), Vec2(31.f, 0.f), 0.1f, 13);

	GetComponent()->GetAnimator()->CreateAnimation(L"JUMP_LEFT", m_pLeftTex, Vec2(0.f, 223.f), Vec2(26.f, 28.f), Vec2(31.f, 0.f), 0.1f, 6);
	GetComponent()->GetAnimator()->CreateAnimation(L"JUMP_RIGHT", m_pRightTex, Vec2(379.f, 223.f), Vec2(26.f, 28.f), Vec2(-31.f, 0.f), 0.1f, 6);


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
	if (KEY_HOLD(KEY::A))
	{
		m_iDir = -1;
		if (m_eCurState != PLAYER_STATE::JUMP)
		{
			m_eCurState = PLAYER_STATE::WALK;
		}
	}
	if (KEY_HOLD(KEY::D))
	{
		m_iDir = 1;
		if (m_eCurState != PLAYER_STATE::JUMP)
		{
			m_eCurState = PLAYER_STATE::WALK;
		}
	}

	if (0.f == GetComponent()->GetRigidbody()->GetSpeed() && m_eCurState != PLAYER_STATE::JUMP)
	{
		m_eCurState = PLAYER_STATE::IDLE;
	}

	if (KEY_TAP(KEY::SPACE))
	{
		m_eCurState = PLAYER_STATE::JUMP;
		GetComponent()->GetRigidbody()->SetVelocity(Vec2(GetComponent()->GetRigidbody()->GetVelocity().x, -400.f));
	}
}

void CPlayer::update_move()
{
	CRigidBody* pRigid = GetComponent()->GetRigidbody();

	if (KEY_HOLD(KEY::A))
	{
		pRigid->AddForce(Vec2(-200.f, 0.f));
	}

	if (KEY_HOLD(KEY::D))
	{
		pRigid->AddForce(Vec2(200.f, 0.f));
	}


	if (KEY_TAP(KEY::A))
	{
		pRigid->SetVelocity(Vec2(-100.f, pRigid->GetVelocity().y));
	}
	if (KEY_TAP(KEY::D))
	{
		pRigid->SetVelocity(Vec2(100.f, pRigid->GetVelocity().y));
	}
}

void CPlayer::update_animation()
{
	if (m_ePrevState == m_eCurState && m_iPrevDir == m_iDir) return;

	switch (m_eCurState)
	{
	case PLAYER_STATE::IDLE:
	{
		if (m_iDir == -1) GetComponent()->GetAnimator()->Play(L"IDLE_LEFT", true);
		else GetComponent()->GetAnimator()->Play(L"IDLE_RIGHT", true);
	}
		break;

	case PLAYER_STATE::WALK:
	{
		if (m_iDir == -1) GetComponent()->GetAnimator()->Play(L"JUMP_LEFT", true);
		else GetComponent()->GetAnimator()->Play(L"JUMP_RIGHT", true);
	}
		break;

	case PLAYER_STATE::ALERT:
		break;

	case PLAYER_STATE::ATTACK:
		break;

	case PLAYER_STATE::JUMP:
	{
		if (m_iDir == -1) GetComponent()->GetAnimator()->Play(L"WALK_LEFT", true);
		else GetComponent()->GetAnimator()->Play(L"WALK_RIGHT", true);
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
		}
	}
}

void CPlayer::render(HDC _dc)
{
	//int iWidth = m_pTex->Width();
	//int iHeight = m_pTex->Height();
		
	//Vec2 vPos = GetPos();

	//BitBlt(_dc
	//	, int(vPos.x - (float)(iWidth / 2.f))
	//	, int(vPos.y - (float)(iHeight / 2.f))
	//	, iWidth
	//	, iHeight
	//	, m_pTex->GetDC()
	//	, 0, 0, SRCCOPY);

	//TransparentBlt(_dc
	//	, int(vPos.x - (float)(iWidth / 2.f))
	//	, int(vPos.y - (float)(iHeight / 2.f))
	//	, iWidth, iHeight
	//	, m_pTex->GetDC()
	//	, 0, 0, iWidth, iHeight
	//	, RGB(255, 0, 255)
	//);




	// 컴포넌트 충돌체, 애니메이션등 렌더링
	component_render(_dc);


	// 알파블렌딩 연습. 임시 텍스처 렌더
	//CTexture* pTex = CResMgr::GetInst()->LoadTexture(L"Player", L"Texture\\Player_A.bmp");

	//Vec2 vPos = GetPos();
	//vPos = CCamera::GetInst()->GetRenderPos(vPos);
	//float width = (float)pTex->Width();
	//float height = (float)pTex->Height();

	//BLENDFUNCTION bf = {};
	//bf.BlendOp = AC_SRC_OVER;
	//bf.BlendFlags = 0;
	//bf.AlphaFormat = AC_SRC_ALPHA;
	//bf.SourceConstantAlpha = 255;

	//AlphaBlend(_dc
	//	, (int)(vPos.x - width / 2.f)
	//	, (int)(vPos.y - height / 2.f)
	//	, (int)(width), (int)(height)
	//	, pTex->GetDC()
	//	, 0, 0, (int)(width), (int)(height)
	//	, bf
	//);
}
