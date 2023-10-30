#include "global.h"
#include "CScene_Tool.h"

#include "CScene.h"
#include "CCollisionMgr.h"

#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CScene_Start.h"

CScene_Tool::CScene_Tool()
{
}

CScene_Tool::~CScene_Tool()
{
}

void CScene_Tool::update()
{
	CScene::update();

	if (KEY_TAP(KEY::ENTER))
	{
		ChangeScene(SCENE_TYPE::START);
	}
}

void CScene_Tool::Enter()
{
}

void CScene_Tool::Exit()
{
	DeleteAll();

	CCollisionMgr::GetInst()->Reset();
}

