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
	m_pTex = CResMgr::GetInst()->LoadTexture(L"PlayerTex", L"texture\\player.bmp");
}

CPlayer::~CPlayer()
{
	//if (nullptr != m_pTex)
	//{
	//	delete m_pTex;
	//}
}

void CPlayer::update()
{
	Vec2 vPos = GetPos();

	if (KEY_HOLD(KEY::W))
	{
		vPos.y -= 100.f * fDT;
	}

	if (KEY_HOLD(KEY::A))
	{
		vPos.x -= 100.f * fDT;
	}

	if (KEY_HOLD(KEY::S))
	{
		vPos.y += 100.f * fDT;
	}

	if (KEY_HOLD(KEY::D))
	{
		vPos.x += 100.f * fDT;
	}

	SetPos(vPos);
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

void CPlayer::render(HDC _dc)
{
	int iWidth = m_pTex->Width();
	int iHeight = m_pTex->Height();

	Vec2 vPos = GetPos();

	//BitBlt(_dc
	//	, int(vPos.x - (float)(iWidth / 2.f))
	//	, int(vPos.y - (float)(iHeight / 2.f))
	//	, iWidth
	//	, iHeight
	//	, m_pTex->GetDC()
	//	, 0, 0, SRCCOPY);

	TransparentBlt(_dc
		, int(vPos.x - (float)(iWidth / 2.f))
		, int(vPos.y - (float)(iHeight / 2.f))
		, iWidth, iHeight
		, m_pTex->GetDC()
		, 0, 0, iWidth, iHeight
		, RGB(255, 0, 255)
	);
}
