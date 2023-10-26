#include "global.h"

#include "CScene_Start.h"
#include "CObject.h"
#include "CPlayer.h"
#include "CMonster.h"

CScene_Start::CScene_Start()
{
}

CScene_Start::~CScene_Start()
{
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

	AddObject(pMonster, GROUP_TYPE::MONSTER);

}

void CScene_Start::Exit()
{
}


