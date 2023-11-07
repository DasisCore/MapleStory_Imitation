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

CPlayer::CPlayer()
	: m_eCurState(PLAYER_STATE::IDLE)
	, m_ePrevState(PLAYER_STATE::WALK)
	, m_iDir(0)
	, m_iPrevDir(0)
{
	// 임시 텍스쳐 로딩
	//m_pTex = new CTexture;

	//wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	//strFilePath += L"texture\\player.bmp";
	//m_pTex->Load(strFilePath);

	// 위 작업을 CResMgr를 이용

	CreateComponent();

	CreateCollider();
	GetComponent()->GetCollider()->SetScale(Vec2(100.f, 100.f));
	GetComponent()->GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));


	CreateRigidbody();
	

	CTexture* pTex = CResMgr::GetInst()->LoadTexture(L"PlayerTex", L"Texture\\link_0.bmp");
	CreateAnimation();
	GetComponent()->GetAnimator()->CreateAnimation(L"WALK_DOWN", pTex, Vec2(0.f, 520.f), Vec2(120.f, 130.f), Vec2(120.f, 0.f), 0.1f, 10);
	GetComponent()->GetAnimator()->Play(L"WALK_DOWN", true);

	CAnimation* pAnim = GetComponent()->GetAnimator()->FindAnimation(L"WALK_DOWN");
	pAnim->GetFrame(0).vOffset = Vec2(0.f, -20.f);
}

CPlayer::~CPlayer()
{
}

void CPlayer::update()
{
	CRigidBody* pRigid = GetComponent()->GetRigidbody();

	Vec2 vPos = GetPos();

	if (KEY_HOLD(KEY::W))
	{
		pRigid->AddForce(Vec2(0.f, -200.f));
	}
	if (KEY_HOLD(KEY::A))
	{
		pRigid->AddForce(Vec2(-200.f, 0.f));
	}
	if (KEY_HOLD(KEY::S))
	{
		pRigid->AddForce(Vec2(0.f, 200.f));
	}
	if (KEY_HOLD(KEY::D))
	{
		pRigid->AddForce(Vec2(200.f, 0.f));
	}


	if (KEY_TAP(KEY::W))
	{
		pRigid->AddVelocity(Vec2(0.f, -100.f));
	}
	if (KEY_TAP(KEY::A))
	{
		pRigid->AddVelocity(Vec2(-100.f, 0.f));
	}
	if (KEY_TAP(KEY::S))
	{
		pRigid->AddVelocity(Vec2(0.f, 100.f));
	}
	if (KEY_TAP(KEY::D))
	{
		pRigid->AddVelocity(Vec2(100.f, 0.f));
	}

	SetPos(vPos);

	GetComponent()->GetAnimator()->update();
	GetComponent()->GetRigidbody();
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

void CPlayer::OnCollisionEnter(CCollider* _pOther)
{
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
