#include "global.h"

#include "CScene_Start.h"
#include "CCore.h"
#include "CObject.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CMonster_Normal.h"

#include "CTexture.h"
#include "CPathMgr.h"

#include "CComponent.h"
#include "CCollisionMgr.h"
#include "CCollider.h"

#include "CSceneMgr.h"
#include "CKeyMgr.h"

#include "CCamera.h"
#include "AI.h"
#include "CIdleState.h"
#include "CTraceState.h"
#include "CPatrolState.h"
#include "CAttackState.h"
#include "CMonFactory.h"

#include "CRigidBody.h"
#include "SelectGDI.h"
#include "CTimeMgr.h"
#include "CGround.h"
#include "CFoothold.h"

#include "CRaven.h"
#include "CVoltarix.h"
#include "CBark.h"

#include "CRandom.h"


CScene_Start::CScene_Start()
	: m_bUseForce(false)
	, m_fForceRadius(500.f)
	, m_fCurRadius(0.f)
	, m_fForce(500.f)
	, m_pTempImage(nullptr)
	, m_itempNumber(0)
{
	SetMapSize(Vec2(1000.f, 1000.f));
}

CScene_Start::~CScene_Start()
{
	// GDI+에서 자원관리를 해주기 때문에 delete를 호출하지 않아도 된다.
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


	//if (KEY_TAP(KEY::ENTER))
	//{
	//	ChangeScene(SCENE_TYPE::TOOL);
	//}

	//if (KEY_TAP(KEY::LBTN))
	//{
	//	Vec2 vLookAt = CCamera::GetInst()->GetRealPos(MOUSE_POS);
	//	CCamera::GetInst()->SetLookAt(vLookAt);
	//}


	if (KEY_TAP(KEY::Z))
	{
		int randomNumber = CRandom::GetInst()->GetBetweenInt<int>(1, 30);
		m_itempNumber = randomNumber;
	}
}

void CScene_Start::render(HDC _dc)
{
	CScene::render(_dc);

	// 임시 아그네스 이미지 출력
	//HDC memDC = CCore::GetInst()->GetMemTex()->GetDC();
	//Graphics graphics(memDC);
	//int Width = m_pTempImage->GetWidth();
	//int Height = m_pTempImage->GetHeight();
	//graphics.DrawImage(m_pTempImage, Width, 0, -Width, Height);
	//graphics.DrawImage(m_pTempImage, 0, Height, Width, Height);

	// 랜덤 함수 테스트
	wstring tempStr = std::to_wstring(m_itempNumber);
	tempStr = L"Random Number : " + tempStr;

	Graphics graphics(_dc);

	Font font(L"Arial", 20, FontStyle::FontStyleBold);

	SolidBrush brush(Color(255, 0, 0, 0));

	PointF point(100.f, 10.f);

	graphics.DrawString(tempStr.c_str(), -1, &font, point, &brush);
	
	// 카메라 위치 랜더링
	Font tfont(L"Arial", 10, FontStyle::FontStyleBold);
	Vec2 cam = CCamera::GetInst()->GetTemp();
	tempStr = std::to_wstring(cam.x);
	graphics.DrawString(tempStr.c_str(), -1, &tfont, PointF(100.f, 50.f), &brush);
	tempStr = std::to_wstring(cam.y);
	graphics.DrawString(tempStr.c_str(), -1, &tfont, PointF(100.f, 60.f), &brush);

	Vec2 vResolution = CCore::GetInst()->GetResolution();
	Pen pen(Color(0, 0, 0), 1);
	graphics.DrawRectangle(&pen, (vResolution.x / 2.f) - 5.f, (vResolution.y / 2.f) - 5.f, 10.f, 10.f);

	// 원그리는 부분
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
	//CObject* pPlayer = new CPlayer;
	//pPlayer->SetName(L"Player");
	//pPlayer->SetPos(Vec2(10040.f, 384.f));
	////pPlayer->SetPos(Vec2(640.f, 384.f));
	//pPlayer->SetScale(Vec2(45.f, 70.f)); 

	//RegisterPlayer(pPlayer);

	//AddObject(pPlayer, GROUP_TYPE::PLAYER);
	//CreateObject(pPlayer, GROUP_TYPE::PLAYER);

	//CObject* pOtherPlayer = pPlayer->Clone();
	//pOtherPlayer->SetPos(Vec2(100.f, 400.f));
	//CreateObject(pOtherPlayer, GROUP_TYPE::PLAYER);

	//CCamera::GetInst()->SetTarget(pPlayer);

	// Voltarix 추가
	CPlayer* pVoltarix = new CBark;
	pVoltarix->SetName(L"RAVEN");
	pVoltarix->SetPos(Vec2(740.f, 383.f));
	pVoltarix->SetScale(Vec2(45.f, 70.f));

	RegisterPlayer(pVoltarix);

	AddObject(pVoltarix, GROUP_TYPE::PLAYER);



	// 몬스터 배치
	Vec2 vResolution = CCore::GetInst()->GetResolution();
	//CMonster* pMon = CMonFactory::CreateMonter(MON_TYPE::NORMAL, vResolution / 2.f - Vec2(0.f, 300.f));
	//pMon->SetName(L"Air_Monster");
	//pMon->SetScale(Vec2(60.f, 60.f));
	//AddObject(pMon, GROUP_TYPE::MONSTER);

	// 이벤트를 이용하여 오브젝트 삽입 예시
	//CreateObject(pOtherPlayer, GROUP_TYPE::PLAYER);

	//CMonster_Normal* pMonLesh = new CMonster_Normal;
	//pMonLesh->SetName(L"Monster_Lesh");
	//pMonLesh->SetScale(Vec2(50.f, 60.f));
	//pMonLesh->SetPos(Vec2(200.f, 200.f));

	//tMonInfo info = {};
	//info.fAtt = 10.f;
	//info.fAttRange = 50.f;
	//info.vRecogRange = Vec2(250.f, 50.f);
	//info.fRecogRange = 300.f;
	//info.fHP = 100.f;
	//info.fSpeed = 300.f;
	//info.iDir = 1;

	//pMonLesh->SetMonInfo(info);

	//AI* pAI = new AI;
	//pAI->AddState(new CIdleState);
	//pAI->AddState(new CTraceState);
	//pAI->AddState(new CPatrolState);
	//pAI->AddState(new CAttackState);
	//
	//pAI->SetCurState(MON_STATE::IDLE);
	//pMonLesh->SetAI(pAI);

	//AddObject(pMonLesh, GROUP_TYPE::MONSTER);


	vector<wstring> MonVecPath 
		= {   L"Animation\\LESH_LEFT_ATTACK.anim"
			, L"Animation\\LESH_LEFT_IDLE.anim"
			, L"Animation\\LESH_LEFT_WALK.anim"
			, L"Animation\\LESH_LEFT_DEAD.anim"
			, L"Animation\\LESH_RIGHT_IDLE.anim"
			, L"Animation\\LESH_RIGHT_WALK.anim"
			, L"Animation\\LESH_RIGHT_ATTACK.anim"
			, L"Animation\\LESH_RIGHT_DEAD.anim"
		};
	CMonster* pMonLesh = CMonFactory::CreateMonster(MON_TYPE::NORMAL, L"LESH", Vec2(200.f, 200.f), Vec2(60.f, 60.f), true, Vec2(0.f, 0.f), Vec2(60.f, 60.f), true, MonVecPath, true, true);
	AddObject(pMonLesh, GROUP_TYPE::MONSTER);


	// 땅 물체 배치
	//CObject* pGround = new CGround;
	//pGround->SetName(L"Ground");
	//pGround->SetPos(Vec2(400.f, 584.f));
	//pGround->SetScale(Vec2(700.f, 60.f));
	//AddObject(pGround, GROUP_TYPE::FOOTHOLD);

	CObject* pGround = new CFoothold;
	pGround->SetName(L"Foothold");
	pGround->SetPos(Vec2(400.f, 584.f));
	pGround->SetScale(Vec2(700.f, 60.f));
	AddObject(pGround, GROUP_TYPE::FOOTHOLD);

	CObject* pFoothold = new CFoothold;
	pFoothold->SetName(L"Foothold");
	pFoothold->SetPos(Vec2(700.f, 184.f));
	pFoothold->SetScale(Vec2(700.f, 60.f));
	AddObject(pFoothold, GROUP_TYPE::FOOTHOLD);


	// 충돌 지정
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::GROUND);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::FOOTHOLD);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::GROUND);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::FOOTHOLD);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::DETECT);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::SKILL);

	// Camera Look 지정
	CCamera::GetInst()->SetLookAt(vResolution / 2.f);
	//CCamera::GetInst()->SetTarget(pPlayer);

	// Camera 효과 지정
	CCamera::GetInst()->FadeOut(1.f);

	// ====================================================================================
	// 임시 GDI+ 이미지 출력
	//wstring strRelativePath = CPathMgr::GetInst()->GetContentPath();
	//strRelativePath += L"\\Texture\\Player\\Raven.png";
	//m_pTempImage = Image::FromFile(strRelativePath.c_str());

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


