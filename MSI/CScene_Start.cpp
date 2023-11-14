#include "global.h"

#include "CScene_Start.h"
#include "CCore.h"
#include "CObject.h"
#include "CPlayer.h"
#include "CMonster.h"

#include "CTexture.h"
#include "CPathMgr.h"

#include "CComponent.h"
#include "CCollisionMgr.h"

#include "CSceneMgr.h"
#include "CKeyMgr.h"

#include "CCamera.h"
#include "AI.h"
#include "CIdleState.h"
#include "CTraceState.h"

#include "CRigidBody.h"
#include "SelectGDI.h"
#include "CTimeMgr.h"
#include "CGround.h"


CScene_Start::CScene_Start()
	: m_bUseForce(false)
	, m_fForceRadius(500.f)
	, m_fCurRadius(0.f)
	, m_fForce(500.f)
	, m_pTempImage(nullptr)
{
}

CScene_Start::~CScene_Start()
{
	//if (m_pTempImage) delete m_pTempImage;
}

void CScene_Start::update()
{
	if (KEY_HOLD(KEY::LBTN))
	{
		//m_bUseForce = true;
		//CreateForce();
	}
	else
	{
		m_bUseForce = false;
	}


	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		const vector<CObject*> vecObj = GetGroupObject((GROUP_TYPE)i);

		for (size_t j = 0; j < vecObj.size(); j++)
		{
			if (vecObj[j]->IsDead()) continue;

			if (m_bUseForce && vecObj[j]->GetComponent()->GetRigidbody())
			{
				m_vForcePos;
				Vec2 vDiff = vecObj[j]->GetPos() - m_vForcePos;
				float fLen = vDiff.Length();
				if (fLen < m_fForceRadius)
				{
					float fRatio = 1.f - (fLen / m_fForceRadius);
					float fForce = m_fForce * fRatio;

					vecObj[j]->GetComponent()->GetRigidbody()->AddForce(vDiff.Normalize() * fForce);
				}
			}
				
			vecObj[j]->update();
		}
	}


	if (KEY_TAP(KEY::ENTER))
	{
		ChangeScene(SCENE_TYPE::TOOL);
	}

	//if (KEY_TAP(KEY::LBTN))
	//{
	//	Vec2 vLookAt = CCamera::GetInst()->GetRealPos(MOUSE_POS);
	//	CCamera::GetInst()->SetLookAt(vLookAt);
	//}


}

void CScene_Start::render(HDC _dc)
{
	CScene::render(_dc);

	// 임시 아그네스 이미지 출력
	HDC memDC = CCore::GetInst()->GetMemTex()->GetDC();
	Graphics graphics(memDC);
	graphics.DrawImage(m_pTempImage, 0, 0);

	if (!m_bUseForce) return;

	SelectGDI a(_dc, BRUSH_TYPE::HOLLOW);
	SelectGDI b(_dc, PEN_TYPE::GREEN);

	m_fCurRadius += m_fForceRadius * 2.f * fDT;
	if (m_fCurRadius > m_fForceRadius)
	{
		m_fCurRadius = 0.f;
	}

	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(m_vForcePos);

	Ellipse(_dc
		, vRenderPos.x - m_fCurRadius
		, vRenderPos.y - m_fCurRadius
		, vRenderPos.x + m_fCurRadius
		, vRenderPos.y + m_fCurRadius);
}

void CScene_Start::Enter()
{
	// 진입시 오브젝트 추가
	CObject* pPlayer = new CPlayer;
	pPlayer->SetName(L"Player");
	pPlayer->SetPos(Vec2(640.f, 384.f));
	pPlayer->SetScale(Vec2(100.f, 100.f)); 

	RegisterPlayer(pPlayer);

	AddObject(pPlayer, GROUP_TYPE::PLAYER);
	//CreateObject(pPlayer, GROUP_TYPE::PLAYER);

	//CObject* pOtherPlayer = pPlayer->Clone();
	//pOtherPlayer->SetPos(Vec2(100.f, 400.f));
	//CreateObject(pOtherPlayer, GROUP_TYPE::PLAYER);


	//CCamera::GetInst()->SetTarget(pPlayer);

	// 몬스터 배치
	Vec2 vResolution = CCore::GetInst()->GetResolution();
	CMonster* pMon = CMonFactory::CreateMonter(MON_TYPE::NORMAL, vResolution / 2.f - Vec2(0.f, 300.f));
	pMon->SetName(L"Monster");
	pMon->SetScale(Vec2(110.f, 60.f));
	AddObject(pMon, GROUP_TYPE::MONSTER);

	// 이벤트를 이용하여 오브젝트 삽입 예시
	//CreateObject(pOtherPlayer, GROUP_TYPE::PLAYER);


	// 땅 물체 배치
	CObject* pGround = new CGround;
	pGround->SetName(L"Ground");
	pGround->SetPos(Vec2(640.f, 584.f));
	pGround->SetScale(Vec2(1000.f, 60.f));
	AddObject(pGround, GROUP_TYPE::GROUND);


	// 충돌 지정
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::GROUND);

	// Camera Look 지정
	//CCamera::GetInst()->SetLookAt(vResolution / 2.f);
	CCamera::GetInst()->SetTarget(pPlayer);

	// Camera 효과 지정
	CCamera::GetInst()->FadeOut(1.f);

	// 임시로 이미지 한장 출력
	wstring strRelativePath = CPathMgr::GetInst()->GetContentPath();
	strRelativePath += L"\\Texture\\Agnes.png";
	m_pTempImage = Image::FromFile(strRelativePath.c_str());

	start();
}

void CScene_Start::Exit()
{
	DeleteAll();

	CCollisionMgr::GetInst()->Reset();
}

void CScene_Start::CreateForce()
{
	m_vForcePos = CCamera::GetInst()->GetRealPos(MOUSE_POS);

}


