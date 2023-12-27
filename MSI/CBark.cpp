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
	if (KEY_TAP(KEY::CTRL))
	{
		CObject* pObj = (CObject*) new CNatureGrab(this);
		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		pCurScene->AddObject(pObj, GROUP_TYPE::SKILL);
		SetDelay(0.25f);
	}

	if (KEY_TAP(KEY::Z))
	{
		CObject* pObj = (CObject*) new CFortress(this);
		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		pCurScene->AddObject(pObj, GROUP_TYPE::SKILL);
	}

	if (KEY_TAP(KEY::X))
	{
		CObject* pObj = (CObject*) new CSeed(this);
		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		pCurScene->AddObject(pObj, GROUP_TYPE::SKILL);
	}

	if (KEY_TAP(KEY::LSHIFT))
	{
		CObject* pObj = (CObject*) new CBlessingSeed(this);
		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		pCurScene->AddObject(pObj, GROUP_TYPE::SKILL);
	}
}

