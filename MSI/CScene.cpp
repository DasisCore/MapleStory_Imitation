#include "global.h"

#include "CCore.h"
#include "CScene.h"
#include "CObject.h"

#include "CResMgr.h"

#include "CTexture.h"
#include "CTile.h"

#include "CPathMgr.h"

#include "CCamera.h"




CScene::CScene()
	: m_iTileX(0)
	, m_iTileY(0)
	, m_pPlayer(nullptr)
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
		if ((UINT)GROUP_TYPE::TILE == i)
		{
			render_tile(_dc);
			continue;
		}


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
}

void CScene::render_tile(HDC _dc)
{
	vector<CObject*> vecTile = GetGroupObject(GROUP_TYPE::TILE);

	Vec2 vCamLook = CCamera::GetInst()->GetLookAt();
	Vec2 vResolution = CCore::GetInst()->GetResolution();

	Vec2 vLeftTop = vCamLook - vResolution / 2.f;

	int iTileSize = TILE_SIZE;

	int iLTCol = (int)vLeftTop.x / iTileSize;
	int iLTRow = (int)vLeftTop.y / iTileSize;

	int iClientWidth = ((int)vResolution.x / iTileSize) + 1;
	int iClientHeigth = ((int)vResolution.y / iTileSize) + 1;

	for (int iCurRow = iLTRow; iCurRow < (iLTRow + iClientHeigth); iCurRow++)
	{
		for (int iCurCol = iLTCol; iCurCol < (iLTCol + iClientWidth); iCurCol++)
		{
			if (iCurCol < 0 || m_iTileX <= iCurCol
				|| iCurRow < 0 || m_iTileY <= iCurRow) continue;

			int iIdx = (m_iTileX * iCurRow) + iCurCol;

			vecTile[iIdx]->render(_dc);
		}
	}

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

void CScene::CreateTile(UINT _iXCount, UINT _iYCount)
{
	DeleteGroup(GROUP_TYPE::TILE);

	m_iTileX = _iXCount;
	m_iTileY = _iYCount;

	CTexture* pTileTex = CResMgr::GetInst()->LoadTexture(L"Tile", L"texture\\tile\\TILE.bmp");

	for (UINT i = 0; i < _iXCount; i++)
	{
		for (UINT j = 0; j < _iYCount; j++)	
		{
			CTile* pTile = new CTile;

			pTile->SetPos(Vec2((float)(j * TILE_SIZE), (float)(i * TILE_SIZE)));
			pTile->SetTexture(pTileTex);

			AddObject(pTile, GROUP_TYPE::TILE);
		}
	}

}

void CScene::LoadTile(const wstring& _strRelativePath)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	FILE* pFile = nullptr;

	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");

	assert(pFile);

	UINT xCount;
	UINT yCount;

	// 타일 가로 세로 개수 불러오기
	fread(&xCount, sizeof(UINT), 1, pFile);
	fread(&yCount, sizeof(UINT), 1, pFile);

	// 타일 생성
	CreateTile(xCount, yCount);

	// 타일들의 개별 데이터 읽어오기
	const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

	for (size_t i = 0; i < vecTile.size(); i++)
	{
		((CTile*)vecTile[i])->Load(pFile);
	}


	fclose(pFile);
}
