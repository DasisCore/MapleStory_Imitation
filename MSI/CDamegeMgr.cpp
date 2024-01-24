#include "global.h"
#include "CDamegeMgr.h"
#include "CTimeMgr.h"
#include "CDamegeFactory.h"
#include "CEffect.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CSoundMgr.h"

CDamegeMgr::CDamegeMgr()
{

}


CDamegeMgr::~CDamegeMgr()
{

}

void CDamegeMgr::init()
{

}

void CDamegeMgr::update()
{
	if (m_vecDamege.size() > 0)
	{
		vector<DAMEGE_INFO> vecTemp;

		for (int i = 0; i < m_vecDamege.size(); i++)
		{
			DAMEGE_INFO& tInfo = m_vecDamege[i];
			if (tInfo.fLatencyTime < 0)
			{
				CDamegeFactory::CreateSingleDamege(tInfo.pObj, tInfo.fDamege, tInfo.fOffset, tInfo.eType);
				CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

				CEffect* pEffect = new CEffect(tInfo.pObj, L"NatureGrabHit", 6, 0.05f, 1);
				pCurScene->AddObject(pEffect, GROUP_TYPE::EFFECT);

				CSoundMgr::GetInst()->Play(L"NATUREGRASPHIT", SOUND_TYPE::EFFECT2);
			}
			else
			{
				tInfo.fLatencyTime -= fDT;
				vecTemp.push_back(tInfo);
			}
		}

		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		vector<CObject*> vecObj = pCurScene->GetGroupObject(GROUP_TYPE::EFFECT);

		m_vecDamege = vecTemp;
	}
}
