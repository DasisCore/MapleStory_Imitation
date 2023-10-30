#include "global.h"

#include "CScene_Start.h"
#include "CObject.h"
#include "CPlayer.h"
#include "CMonster.h"

#include "CTexture.h"
#include "CPathMgr.h"

#include "CCollisionMgr.h"

#include "CSceneMgr.h"
#include "CKeyMgr.h"

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
}

void CScene_Start::Enter()
{
	// 진입시 오브젝트 추가
	CObject* pPlayer = new CPlayer;
	pPlayer->SetName(L"First Player");
	pPlayer->SetPos(Vec2(640.f, 384.f));
	pPlayer->SetScale(Vec2(100.f, 100.f));

	AddObject(pPlayer, GROUP_TYPE::PLAYER);

	// 진입시 오브젝트 추가
	CObject* pMonster = new CMonster;
	pMonster->SetName(L"First Monster");
	pMonster->SetPos(Vec2(640.f, 100.f));
	pMonster->SetScale(Vec2(100.f, 50.f));

	//AddObject(pMonster, GROUP_TYPE::MONSTER);


	// 오브젝트 추가하는 이벤트
	CreateObject(pMonster, GROUP_TYPE::MONSTER);


	// 충돌 지정
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);

}

void CScene_Start::Exit()
{
	DeleteAll();

	CCollisionMgr::GetInst()->Reset();
}


