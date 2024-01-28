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

#include "CDamegeFactory.h"
#include "CEffect.h"

#include "CSoundMgr.h"
#include "CPathMgr.h"

CScene_Test::CScene_Test()
{

}

CScene_Test::~CScene_Test()
{
}


void CScene_Test::update()
{
	CScene::update();

	//if (KEY_TAP(KEY::ENTER))
	//{
	//	int damege = CRandom::GetInst()->GetBetweenInt(300, 50000);
	//	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	//	CObject* pPlayer = pCurScene->GetPlayer();
	//	CDamegeFactory::CreateSingleDamege(pPlayer, damege, DAMEGE_TYPE::VIOLET);
	//}

	//if (KEY_TAP(KEY::ENTER))
	//{
	//	CEffect* pObj = new CEffect(6, 0.05f);
	//	AddObject(pObj, GROUP_TYPE::EFFECT);
	//}


	if (KEY_TAP(KEY::ENTER))
	{

		wstring contentPath = CPathMgr::GetInst()->GetContentPath();
		vector<wstring> animationPath;

		animationPath.push_back((L"Animation\\LESH_LEFT_IDLE.anim"));
		animationPath.push_back((L"Animation\\LESH_LEFT_ATTACK.anim"));
		animationPath.push_back((L"Animation\\LESH_LEFT_DEAD.anim"));
		animationPath.push_back((L"Animation\\LESH_LEFT_HIT.anim"));
		animationPath.push_back((L"Animation\\LESH_LEFT_WALK.anim"));

		animationPath.push_back((L"Animation\\LESH_RIGHT_IDLE.anim"));
		animationPath.push_back((L"Animation\\LESH_RIGHT_ATTACK.anim"));
		animationPath.push_back((L"Animation\\LESH_RIGHT_DEAD.anim"));
		animationPath.push_back((L"Animation\\LESH_RIGHT_HIT.anim"));
		animationPath.push_back((L"Animation\\LESH_RIGHT_WALK.anim"));


		CObject* pMon = (CObject*) CMonFactory::CreateMonster(MON_TYPE::NORMAL, L"LESH", Vec2(0.f, 0.f), Vec2(80.f, 80.f), true, Vec2(0.f, 0.f), Vec2(80.f, 100.f), true, animationPath, true, true);
		AddObject(pMon, GROUP_TYPE::MONSTER);
	}


}

void CScene_Test::Enter()
{
	//LoadSceneData(L"TEST.scene");
	LoadSceneData(L"STAGE01.scene");
	MonsterGenCountCheck();

	// 플레이어 삽입
	CPlayer* pVoltarix = new CBark;
	pVoltarix->SetName(L"PLAYER");
	pVoltarix->SetPos(Vec2(0.f, 0.f));
	pVoltarix->SetScale(Vec2(45.f, 70.f));
	RegisterPlayer(pVoltarix);

	AddObject(pVoltarix, GROUP_TYPE::PLAYER);

	
	// 게임 ui 삽입.

	Vec2 vResolution = CCore::GetInst()->GetResolution();

	CUI* pPanelUI = new CSkillUI(L"Texture\\SkillBar\\Skillbar.png");
	pPanelUI->SetName(L"SKILLBAR");
	pPanelUI->SetPos(Vec2(vResolution.x / 2.f - pPanelUI->GetScale().x / 2.f, vResolution.y - pPanelUI->GetScale().y - 20));
	AddObject(pPanelUI, GROUP_TYPE::UI);


	// 충돌 지정
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::GROUND);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::FOOTHOLD);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::GROUND);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::FOOTHOLD);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::DETECT);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::SKILL);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::SKILL);


	CCamera::GetInst()->SetTarget(pVoltarix);

	CSoundMgr::GetInst()->CreateSound(L"MysteriousJourney", "MysteriousJourney.mp3", true);
	CSoundMgr::GetInst()->Play(L"MysteriousJourney", SOUND_TYPE::BGM);
}

void CScene_Test::Exit()
{
}
