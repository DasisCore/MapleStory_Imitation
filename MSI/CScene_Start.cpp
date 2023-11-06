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

CScene_Start::CScene_Start()
{
}

CScene_Start::~CScene_Start()
{
}

void CScene_Start::update()
{
	// 부모가 가지고 있는 update() 먼저 호출
	CScene::update();

	if (KEY_TAP(KEY::ENTER))
	{
		ChangeScene(SCENE_TYPE::TOOL);
	}

	if (KEY_TAP(KEY::LBTN))
	{
		Vec2 vLookAt = CCamera::GetInst()->GetRealPos(MOUSE_POS);
		CCamera::GetInst()->SetLookAt(vLookAt);
	}

}

void CScene_Start::Enter()
{
	// 진입시 오브젝트 추가
	CObject* pPlayer = new CPlayer;
	pPlayer->SetName(L"First Player");
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
	AddObject(pMon, GROUP_TYPE::MONSTER);

	// 이벤트를 이용하여 오브젝트 삽입 예시
	//CreateObject(pOtherPlayer, GROUP_TYPE::PLAYER);


	// 충돌 지정
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);

	// Camera Look 지정
	CCamera::GetInst()->SetLookAt(vResolution / 2.f);

	// Camera 효과 지정
	CCamera::GetInst()->FadeOut(1.f);

}

void CScene_Start::Exit()
{
	DeleteAll();

	CCollisionMgr::GetInst()->Reset();
}


