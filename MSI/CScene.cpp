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


CScene::CScene()
	: m_pPlayer(nullptr)
{
}

CScene::~CScene()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_vecObj[i].size(); j++)
		{
			// m_vecObj[i] �׷� ������ j ������Ʈ ����
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
}

void CScene::finalupdate()
{
	//if (CSceneMgr::GetInst()->GetCurScene()->GetName() == L"Tool Scene")
	if (1)
	{
		// ���̳� ������Ʈ ����, ������Ʈ Ÿ���� ����
		bool flag = 0;
		CObject* pObj = nullptr;
		int idx = -1;
		for (int i = (UINT)GROUP_TYPE::END - 1; i > -1; i--)
		{
			for (int j = m_vecObj[i].size() - 1; j > -1; j--)
			{
				if (flag == 1)
				{
					m_vecObj[i][j]->SetTarget(false);
				}

				if (!flag && CKeyMgr::GetInst()->IsMouseInObj(m_vecObj[i][j]))
				{
					m_vecObj[i][j]->SetTarget(true);
					pObj = m_vecObj[i][j];
					idx = i;
					flag = 1;
				}
			}
		}

		// Ÿ���� �� ������Ʈ�� �� Object�� ������ �� �ڷ� �ű��.
		if (idx != -1 && pObj != nullptr)
		{
			m_vecObj[idx].erase(std::remove(m_vecObj[idx].begin(), m_vecObj[idx].end(), pObj), m_vecObj[idx].end());
			m_vecObj[idx].push_back(pObj);
		}
	}


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
	// �⺻���� ������Ʈ ������ (���� �߿�)
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


	// �� ũ�� �����ؼ� �׸���
	Pen pen(Color(240, 240, 240), 2);
	Vec2 vCenter = m_vMapSize / 2.f;
	vCenter = CCamera::GetInst()->GetRenderPos(vCenter);
	graphics.DrawRectangle(&pen, Rect(vCenter.x - (m_vMapSize.x / 2.f), vCenter.y - (m_vMapSize.y / 2.f), m_vMapSize.x, m_vMapSize.y));
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

	// �� ũ�� �б� 
	UINT width = 0;
	UINT height = 0;
	fread(&width, sizeof(UINT), 1, pFile);
	fread(&height, sizeof(UINT), 1, pFile);

	// �� ũ�� ����
	m_vMapSize = Vec2((float)width, (float)height);

	// Background1 �б�.
	UINT vecSize = 0;
	fread(&vecSize, sizeof(UINT), 1, pFile);
	for (int i = 0; i < vecSize; i++)
	{
		CObject* pObj = ReadObject<CBackground>(pFile);
		AddObject(pObj, GROUP_TYPE::BACKGROUND1);
	}

	// Background2 �б�.
	vecSize = 0;
	fread(&vecSize, sizeof(UINT), 1, pFile);
	for (int i = 0; i < vecSize; i++)
	{
		CObject* pObj = ReadObject<CBackground>(pFile);
		AddObject(pObj, GROUP_TYPE::BACKGROUND2);
	}

	// Background3 �б�.
	vecSize = 0;
	fread(&vecSize, sizeof(UINT), 1, pFile);
	for (int i = 0; i < vecSize; i++)
	{
		CObject* pObj = ReadObject<CBackground>(pFile);
		AddObject(pObj, GROUP_TYPE::BACKGROUND3);
	}

	// FootHold �б�.
	vecSize = 0;
	fread(&vecSize, sizeof(UINT), 1, pFile);
	for (int i = 0; i < vecSize; i++)
	{
		CObject* pObj = ReadObject<CFoothold>(pFile);
		AddObject(pObj, GROUP_TYPE::FOOTHOLD);
	}

	// Ground �б�.
	vecSize = 0;
	fread(&vecSize, sizeof(UINT), 1, pFile);
	for (int i = 0; i < vecSize; i++)
	{
		CObject* pObj = ReadObject<CGround>(pFile);
		AddObject(pObj, GROUP_TYPE::GROUND);
	}

	// Monster �б�. // Monster�� ��쿡�� Factory�� ���ľ��ϹǷ�, �ϵ��ڵ�����.
	vecSize = 0;
	fread(&vecSize, sizeof(UINT), 1, pFile);
	for (int i = 0; i < vecSize; i++)
	{
		// ������Ʈ �̸� �б�.
		wstring strName;
		LoadWString(strName, pFile);

		// ������Ʈ ��ġ
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
			// �ִϸ��̼� ��� ���� �б�
			UINT animSize = 0;
			fread(&animSize, sizeof(UINT), 1, pFile);

			// ����� ������ŭ
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
}
