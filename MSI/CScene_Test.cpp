#include "global.h"
#include "CScene_Test.h"

#include "CPlayer.h"
#include "CBark.h"
#include "CCollisionMgr.h"

CScene_Test::CScene_Test()
{
}

CScene_Test::~CScene_Test()
{
}


void CScene_Test::Enter()
{
	LoadSceneData(L"STAGE01.scene");

	CPlayer* pVoltarix = new CBark;
	pVoltarix->SetName(L"RAVEN");
	pVoltarix->SetPos(Vec2(740.f, 383.f));
	pVoltarix->SetScale(Vec2(45.f, 70.f));

	RegisterPlayer(pVoltarix);

	AddObject(pVoltarix, GROUP_TYPE::PLAYER);

	// 충돌 지정
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::GROUND);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::FOOTHOLD);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::GROUND);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::FOOTHOLD);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::DETECT);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::SKILL);
}

void CScene_Test::Exit()
{
}
