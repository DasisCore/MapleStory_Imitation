#include "global.h"

#include "CCore.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CObject.h"

#include "CResMgr.h"

#include "CTexture.h"
#include "CTile.h"

#include "CPathMgr.h"

#include "CCamera.h"
#include "CKeyMgr.h"
#include "CGround.h"
#include "CFoothold.h"
#include "CBackground.h"
#include "CTimeMgr.h"
#include "CRandom.h"
#include "CMonster.h"
#include "CComponent.h"
#include "CAnimator.h"
#include "CCollider.h"
#include "CKeyMgr.h"


CScene::CScene()
	: m_pPlayer(nullptr)
	, m_iMonGen(0)
	, m_GenCycle(5.f)
{
}

CScene::~CScene()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_vecObj[i].size(); j++)
		{
			// m_vecObj[i] 그룹 벡터의 j 오브젝트 삭제
			delete m_vecObj[i][j];
		}
	}

}

void CScene::start()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_vecObj[i].size(); j++)
		{
			m_vecObj[i][j]->start();
		}
	}
}

void CScene::update()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_vecObj[i].size(); j++)
		{
			if (m_vecObj[i][j]->IsDead()) continue;
			m_vecObj[i][j]->update();
		}
	}

	if (m_GenCycle < 0)
	{
		MonGenerate();
		m_GenCycle = 7.f;
	}
	m_GenCycle -= fDT;

	if (KEY_TAP(KEY::F1))
	{
		CCore::GetInst()->FlipRenderOption();
	}
}

void CScene::finalupdate()
{
	//if (CSceneMgr::GetInst()->GetCurScene()->GetName() == L"Tool Scene")
	//if (1)
	//{
	//	// 파이널 업데이트 전에, 오브젝트 타겟팅 설정
	//	bool flag = 0;
	//	CObject* pObj = nullptr;
	//	int idx = -1;

	//	//for (int i = (UINT)GROUP_TYPE::END - 1; i > -1; i--)
	//	//{
	//	//	for (int j = m_vecObj[i].size() - 1; j > -1; j--)
	//	//	{
	//	//		if (flag == 1)
	//	//		{
	//	//			m_vecObj[i][j]->SetTarget(false);
	//	//		}

	//	//		if (!flag && CKeyMgr::GetInst()->IsMouseInObj(m_vecObj[i][j]))
	//	//		{
	//	//			m_vecObj[i][j]->SetTarget(true);
	//	//			pObj = m_vecObj[i][j];
	//	//			idx = i;
	//	//			flag = 1;
	//	//		}
	//	//	}
	//	//}

	//	// 타겟팅 된 오브젝트를 맨 Object의 벡터의 맨 뒤로 옮긴다.
	//	if (idx != -1 && pObj != nullptr)
	//	{
	//		m_vecObj[idx].erase(std::remove(m_vecObj[idx].begin(), m_vecObj[idx].end(), pObj), m_vecObj[idx].end());
	//		m_vecObj[idx].push_back(pObj);
	//	}
	//}


	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_vecObj[i].size(); j++)
		{
			m_vecObj[i][j]->finalupdate();
		}
	}
}

void CScene::render(HDC _dc)
{
	// 기본적인 오브젝트 랜더링 (제일 중요)
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		vector<CObject*>::iterator iter = m_vecObj[i].begin();

		for (; iter != m_vecObj[i].end();)
		{
			if (!(*iter)->IsDead())
			{
				(*iter)->render(_dc);
				iter++;
			}
			else
			{
				iter = m_vecObj[i].erase(iter);
			}
		}
	}

	Graphics graphics(_dc);
	Font font(L"Arial", 12, FontStyleBold);
	SolidBrush brush(Color(255, 0, 0, 0));
	graphics.DrawString(m_strName.c_str(), -1, &font, PointF(10, 10), &brush);


	//// 맵 크기 관련해서 그리기
	//Pen pen(Color(240, 240, 240), 2);
	//Vec2 vCenter = m_vMapSize / 2.f;
	//vCenter = CCamera::GetInst()->GetRenderPos(-vCenter);
	//graphics.DrawRectangle(&pen, Rect(vCenter.x, vCenter.y, m_vMapSize.x, m_vMapSize.y));

	//Vec2 vCenter2 = Vec2(0.f, 0.f);
	//vCenter2 = CCamera::GetInst()->GetRenderPos(vCenter2);
	//Pen pen2(Color(0, 0, 0), 3.f);
	//graphics.DrawRectangle(&pen2, vCenter2.x - 2.5f, vCenter2.y - 2.5f, 5.f, 5.f);
}


void CScene::DeleteGroup(GROUP_TYPE _eTarget)
{
	Safe_Delete_Vector<CObject*>(m_vecObj[(UINT)_eTarget]);
}

void CScene::DeleteAll()
{
	for(UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		DeleteGroup((GROUP_TYPE)i);
	}
}

void CScene::DeleteAll_Except_UI()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		if (i == (UINT)GROUP_TYPE::UI) continue;
		DeleteGroup((GROUP_TYPE)i);
	}
}

void CScene::LoadSceneData(const wstring& _strFilePath)
{
	FILE* pFile = nullptr;
	wstring absolutePath = CPathMgr::GetInst()->GetContentPath();

	absolutePath += L"Scene\\" + _strFilePath;

	_wfopen_s(&pFile, absolutePath.c_str(), L"rb");
	assert(pFile);

	// 맵 크기 읽기 
	UINT width = 0;
	UINT height = 0;
	fread(&width, sizeof(UINT), 1, pFile);
	fread(&height, sizeof(UINT), 1, pFile);

	// 맵 크기 적용
	m_vMapSize = Vec2((float)width, (float)height);

	// Background1 읽기.
	UINT vecSize = 0;
	fread(&vecSize, sizeof(UINT), 1, pFile);
	for (int i = 0; i < vecSize; i++)
	{
		CObject* pObj = ReadObject<CBackground>(pFile);
		AddObject(pObj, GROUP_TYPE::BACKGROUND1);
	}

	// Background2 읽기.
	vecSize = 0;
	fread(&vecSize, sizeof(UINT), 1, pFile);
	for (int i = 0; i < vecSize; i++)
	{
		CObject* pObj = ReadObject<CBackground>(pFile);
		AddObject(pObj, GROUP_TYPE::BACKGROUND2);
	}

	// Background3 읽기.
	vecSize = 0;
	fread(&vecSize, sizeof(UINT), 1, pFile);
	for (int i = 0; i < vecSize; i++)
	{
		CObject* pObj = ReadObject<CBackground>(pFile);
		AddObject(pObj, GROUP_TYPE::BACKGROUND3);
	}

	// FootHold 읽기.
	vecSize = 0;
	fread(&vecSize, sizeof(UINT), 1, pFile);
	for (int i = 0; i < vecSize; i++)
	{
		CObject* pObj = ReadObject<CFoothold>(pFile);
		AddObject(pObj, GROUP_TYPE::FOOTHOLD);
	}

	// Ground 읽기.
	vecSize = 0;
	fread(&vecSize, sizeof(UINT), 1, pFile);
	for (int i = 0; i < vecSize; i++)
	{
		CObject* pObj = ReadObject<CGround>(pFile);
		AddObject(pObj, GROUP_TYPE::GROUND);
	}

	// Monster 읽기. // Monster의 경우에는 Factory를 거쳐야하므로, 하드코딩했음.
	vecSize = 0;
	fread(&vecSize, sizeof(UINT), 1, pFile);
	for (int i = 0; i < vecSize; i++)
	{
		// 오브젝트 이름 읽기.
		wstring strName;
		LoadWString(strName, pFile);

		// 오브젝트 위치
		float fPosX = 0.f;
		float fPosY = 0.f;
		fread(&fPosX, sizeof(float), 1, pFile);
		fread(&fPosY, sizeof(float), 1, pFile);
		Vec2 vPos = Vec2(fPosX, fPosY);

		float fScaleX = 0.f;
		float fScaleY = 0.f;
		fread(&fScaleX, sizeof(float), 1, pFile);
		fread(&fScaleY, sizeof(float), 1, pFile);
		Vec2 vScale = Vec2(fScaleX, fScaleY);

		bool bCollider = false;
		fread(&bCollider, sizeof(bool), 1, pFile);
		Vec2 vColOffset = Vec2(0.f, 0.f);
		Vec2 vColScale = Vec2(0.f, 0.f);

		if (bCollider == true)
		{
			float fOffsetX = 0.f;
			float fOffsetY = 0.f;
			fread(&fOffsetX, sizeof(float), 1, pFile);
			fread(&fOffsetY, sizeof(float), 1, pFile);
			vColOffset = Vec2(fOffsetX, fOffsetY);

			float fColScaleX = 0.f;
			float fColScaleY = 0.f;
			fread(&fColScaleX, sizeof(float), 1, pFile);
			fread(&fColScaleY, sizeof(float), 1, pFile);
			vColScale = Vec2(fColScaleX, fColScaleY);
		}

		bool bAnimator = false;
		fread(&bAnimator, sizeof(bool), 1, pFile);

		vector<wstring> vecPath;
		if (bAnimator == true)
		{
			// 애니메이션 경로 갯수 읽기
			UINT animSize = 0;
			fread(&animSize, sizeof(UINT), 1, pFile);

			// 경로의 갯수만큼
			for (int i = 0; i < animSize; i++)
			{
				wstring animPath = L"";
				LoadWString(animPath, pFile);
				vecPath.push_back(animPath);
			}
		}

		bool bRigidBody = false;
		fread(&bRigidBody, sizeof(bool), 1, pFile);

		bool bGravity = false;
		fread(&bGravity, sizeof(bool), 1, pFile);

		CObject* pMon = (CObject*)CMonFactory::CreateMonster(MON_TYPE::NORMAL, strName, vPos, vScale, bCollider, vColOffset, vColScale, bAnimator, vecPath, bGravity, bRigidBody);
		AddObject(pMon, GROUP_TYPE::MONSTER);
	}

	fclose(pFile);
}

void CScene::MonsterGenCountCheck()
{
	vector<CObject*> vecObj = GetGroupObject(GROUP_TYPE::MONSTER);
	
	for (int i = 0; i < vecObj.size(); i++)
	{
		MON_GEN_INFO pGenInfo;
		CMonster* pObj = (CMonster*)vecObj[i];
		pGenInfo.strName = pObj->GetName();
		pGenInfo.vPos = pObj->GetPos();
		pGenInfo.vScale = pObj->GetScale();
		pGenInfo.bCollider = pObj->GetComponent()->GetCollider() == nullptr ? false : true;
		pGenInfo.vColOffset = pObj->GetComponent()->GetCollider() == nullptr ? Vec2(0.f, 0.f) : pObj->GetComponent()->GetCollider()->GetOffsetPos();
		pGenInfo.vColScale = pObj->GetComponent()->GetCollider() == nullptr ? Vec2(0.f, 0.f) : pObj->GetComponent()->GetCollider()->GetScale();
		pGenInfo.bAnimaiton = pObj->GetComponent()->GetAnimator() == nullptr ? false : true;

		map<wstring, CAnimation*> mapAnimation = pObj->GetComponent()->GetAnimator()->GetMapAnim();
		map<wstring, CAnimation*>::iterator iter = mapAnimation.begin();

		if (pObj->GetComponent()->GetAnimator() != nullptr)
		{
			for (; iter != mapAnimation.end(); iter++) pGenInfo.vecPath.push_back(L"\\Animation\\" + iter->first + L".anim");
		}
		pGenInfo.bGravity = pObj->GetComponent()->GetGravity() == nullptr ? false : true;
		pGenInfo.bRigidBody = pObj->GetComponent()->GetRigidbody() == nullptr ? false : true;

		m_vecMonGenData.push_back(pGenInfo);
	}
	
	m_iMonGen = vecObj.size();
}

void CScene::MonGenerate()
{
	int GenCnt = m_iMonGen - GetGroupObject(GROUP_TYPE::MONSTER).size();
	if (GenCnt > 0)
	{
		vector<int> randomMonGenPos = CRandom::GetInst()->GetBetweenIntCnt(0, m_vecMonGenData.size(), GenCnt);

		for (int i = 0; i < randomMonGenPos.size(); i++)
		{
			MON_GEN_INFO sInfo = m_vecMonGenData[i];
			CMonster* pObj = CMonFactory::CreateMonster(MON_TYPE::NORMAL
								, sInfo.strName, sInfo.vPos, sInfo.vScale
								, sInfo.bCollider, sInfo.vColOffset, sInfo.vColScale
								, sInfo.bAnimaiton, sInfo.vecPath
								, sInfo.bGravity, sInfo.bRigidBody);
			AddObject(pObj, GROUP_TYPE::MONSTER);
		}
	}

}
