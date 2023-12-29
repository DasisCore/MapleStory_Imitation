#include "global.h"
#include "CBark.h"

#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CScene.h"

#include "CNatureGrab.h"
#include "CFortress.h"
#include "CBlessingSeed.h"
#include "CSeed.h"

CBark::CBark()
{
}

CBark::~CBark()
{
}

void CBark::update()
{
	CPlayer::update();

	// 스킬 사용할때 딜레이 적용.
	// 자연의 손아귀
	if (KEY_TAP(KEY::CTRL) && GetCanOtherAction() && GetIsGround())
	{
		SetCurState(PLAYER_STATE::ATTACK);
		CObject* pObj = (CObject*) new CNatureGrab(this);
		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		pCurScene->AddObject(pObj, GROUP_TYPE::SKILL);
		SetDelay(0.53f);
	}

	// 요새
	if (KEY_TAP(KEY::Z))
	{
		CObject* pObj = (CObject*) new CFortress(this);
		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		pCurScene->AddObject(pObj, GROUP_TYPE::SKILL);
	}

	// 축복의 씨앗
	if (KEY_TAP(KEY::LSHIFT) && GetCanOtherAction() && GetIsGround())
	{
		SetCurState(PLAYER_STATE::ATTACK);
		CObject* pObj = (CObject*) new CBlessingSeed(this);
		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		pCurScene->AddObject(pObj, GROUP_TYPE::SKILL);
		SetDelay(0.4f);
	}

}

