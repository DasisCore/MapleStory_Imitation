#include "global.h"
#include "CBark.h"

#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CScene.h"

#include "CNatureGrab.h"
#include "CFortress.h"
#include "CBlessingSeed.h"
#include "CSeed.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CSkillUI.h"

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
		MinusMP(1);
	}

	// 요새
	if (KEY_TAP(KEY::Z))
	{
		vector<CObject*> vecObj = CSceneMgr::GetInst()->GetCurScene()->GetGroupObject(GROUP_TYPE::UI);

		CSkillUI* pSkillUI = nullptr;
		for (int i = 0; i < vecObj.size(); i++)
		{
			if (vecObj[i]->GetName() == L"SKILLBAR")
			{
				pSkillUI = dynamic_cast<CSkillUI*> (vecObj[i]);
				break;
			}
		}

		if (pSkillUI != nullptr)
		{
			float fSkillCoolTime = pSkillUI->GetCoolTimeSkill2();
			if (fSkillCoolTime <= 0)
			{
				CObject* pObj = (CObject*) new CFortress(this);
				CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
				pCurScene->AddObject(pObj, GROUP_TYPE::SKILL);
				MinusMP(5);
			}
		}
	}

	// 축복의 씨앗
	if (KEY_TAP(KEY::LSHIFT) && GetCanOtherAction() && GetIsGround())
	{
		vector<CObject*> vecObj = CSceneMgr::GetInst()->GetCurScene()->GetGroupObject(GROUP_TYPE::UI);

		CSkillUI* pSkillUI = nullptr;
		for (int i = 0; i < vecObj.size(); i++)
		{
			if (vecObj[i]->GetName() == L"SKILLBAR")
			{
				pSkillUI = dynamic_cast<CSkillUI*> (vecObj[i]);
				break;
			}
		}

		if (pSkillUI != nullptr)
		{
			float fSkillCoolTime = pSkillUI->GetCoolTimeSkill1();
			if (fSkillCoolTime <= 0)
			{
				SetCurState(PLAYER_STATE::ATTACK);
				CObject* pObj = (CObject*) new CBlessingSeed(this);
				CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
				pCurScene->AddObject(pObj, GROUP_TYPE::SKILL);
				SetDelay(0.7f);
				MinusMP(5);
			}
		}
	}

}

