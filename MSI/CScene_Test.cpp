#include "global.h"
#include "CScene_Test.h"

#include "CPlayer.h"
#include "CBark.h"
#include "CCollisionMgr.h"

#include "CKeyMgr.h"
#include "CCamera.h"

#include "CDamege.h"
#include "CCore.h"

#include "CSceneMgr.h"
#include "CScene.h"

#include "CRandom.h"
#include "CUI.h"
#include "CPanelUI.h"
#include "CSkillUI.h"
#include "CBtnUI.h"


CScene_Test::CScene_Test()
{
}

CScene_Test::~CScene_Test()
{
}


void CScene_Test::update()
{
	CScene::update();

	if (KEY_TAP(KEY::ENTER))
	{

		int damege = CRandom::GetInst()->GetBetweenInt(300, 50000);
		CObject* pObj = new CDamege(damege);
		pObj->SetName(L"Damege");

		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		CObject* pPlayer = pCurScene->GetPlayer();
		Vec2 vPos = pPlayer->GetPos();
		Vec2 vScale = pPlayer->GetScale();
		pObj->SetPos(Vec2(vPos.x + (vScale.x / 2.f), vPos.y - (vScale.y / 2.f)));
		AddObject(pObj, GROUP_TYPE::DAMEGE);
	}
}

void CScene_Test::Enter()
{
	LoadSceneData(L"TEST.scene");

	// 플레이어 삽입
	CPlayer* pVoltarix = new CBark;
	pVoltarix->SetName(L"RAVEN");
	pVoltarix->SetPos(Vec2(0.f, 0.f));
	pVoltarix->SetScale(Vec2(45.f, 70.f));
	RegisterPlayer(pVoltarix);

	AddObject(pVoltarix, GROUP_TYPE::PLAYER);

	
	// 게임 ui 삽입.

	Vec2 vResolution = CCore::GetInst()->GetResolution();

	CUI* pPanelUI = new CSkillUI(L"Texture\\SkillBar\\Skillbar.png");
	pPanelUI->SetName(L"SkillBar");
	pPanelUI->SetPos(Vec2(vResolution.x - pPanelUI->GetScale().x, 0.f));

	//CBtnUI* pBtnUI = new CBtnUI;
	//pBtnUI->SetName(L"Create MAP");
	//pBtnUI->SetScale(Vec2(55.f, 25.f));
	//pBtnUI->SetPos(Vec2(5.f, 5.f));
	//pPanelUI->AddChild(pBtnUI);

	AddObject(pPanelUI, GROUP_TYPE::UI);



	// 충돌 지정
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::GROUND);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::FOOTHOLD);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::GROUND);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::FOOTHOLD);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::DETECT);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::SKILL);


	//CCamera::GetInst()->SetLookAt(pVoltarix->GetPos());
	CCamera::GetInst()->SetTarget(pVoltarix);
	
}

void CScene_Test::Exit()
{
}
