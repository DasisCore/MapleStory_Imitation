#include "global.h"
#include "CDamegeMgr.h"
#include "CTimeMgr.h"
#include "CDamegeFactory.h"
#include "CEffect.h"
#include "CSceneMgr.h"
#include "CScene.h"

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
			DAMEGE_INFO tInfo = m_vecDamege[i];
			if (tInfo.flatencyTime < 0)
			{
				CDamegeFactory::CreateSingleDamege(tInfo.pObj, tInfo.fDamege, tInfo.fOffset, tInfo.eType);
				CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

				CEffect* pEffect = new CEffect(tInfo.pObj, 6, 0.05f);
				pCurScene->AddObject(pEffect, GROUP_TYPE::EFFECT);
			}
			else
			{
				tInfo.flatencyTime -= fDT;
				vecTemp.push_back(tInfo);
			}
		}
		m_vecDamege = vecTemp;
	}
}
