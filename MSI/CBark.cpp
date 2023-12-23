#include "global.h"
#include "CBark.h"

#include "CKeyMgr.h"
#include "CNatureGrab.h"
#include "CSceneMgr.h"
#include "CScene.h"

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
}

