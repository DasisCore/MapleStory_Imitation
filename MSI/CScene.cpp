#include "global.h"

#include "CScene.h"
#include "CObject.h"


CScene::CScene()
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

void CScene::update()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_vecObj[i].size(); j++)
		{
			m_vecObj[i][j]->update();
		}
	}
}

void CScene::finalupdate()
{
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
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_vecObj[i].size(); j++)
		{
			m_vecObj[i][j]->render(_dc);
		}
	}

	RECT rc = { 0, 0, 100, 50 };
	DrawText(_dc, m_strName.c_str(), -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}
