#include "global.h"
#include "CScene_Tool.h"

#include "CScene.h"
#include "CCollisionMgr.h"

#include "CCore.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CScene_Start.h"
#include "CCamera.h"
#include "CTile.h"

#include "CResMgr.h"

#include "resource.h"
#include "CPanelUI.h"
#include "CBtnUI.h"
#include "CUIMgr.h"

#include "CPathMgr.h"

#include "CToolWindow.h"

void ChangeScene(DWORD_PTR, DWORD_PTR);

CScene_Tool::CScene_Tool()
	:m_bGizmo(false)
{
}

CScene_Tool::~CScene_Tool()
{

}


void CScene_Tool::Enter()
{
	// 메뉴 바 붙이기
	CCore::GetInst()->DockMenu();

	// 타일 생성
	//CreateTile(5, 5);

	// UI 하나 만들어보기
	Vec2 vResolution = CCore::GetInst()->GetResolution();

	//CUI* pPanelUI = new CPanelUI;
	//pPanelUI->SetName(L"ParentUI");
	//pPanelUI->SetScale(Vec2(500.f, 300.f));
	//pPanelUI->SetPos(Vec2(vResolution.x - pPanelUI->GetScale().x, 0.f));

	//CBtnUI* pBtnUI = new CBtnUI;
	//pBtnUI->SetName(L"ChildUI");
	//pBtnUI->SetScale(Vec2(100.f, 40.f));  
	//pBtnUI->SetPos(Vec2(0.f, 0.f));
	////pBtnUI->SetClickCallBack(ChangeScene, 0, 0);
	//// tool4
	//pBtnUI->SetClickCallBack(this, (SCENE_MEMFUNC)&CScene_Tool::SaveTileData);

	//pPanelUI->AddChild(pBtnUI);

	//AddObject(pPanelUI, GROUP_TYPE::UI);

	//CUI* pClonePanel = pPanelUI->Clone();
	//pClonePanel->SetPos(pClonePanel->GetPos() + Vec2(-300.f , 0.f));
	//AddObject(pClonePanel, GROUP_TYPE::UI);

	//m_pUI = pClonePanel;


	// 카메라 LookAt 지정
	CCamera::GetInst()->SetLookAt(vResolution / 2.f);

	CToolWindow::GetInst()->init();
	CToolWindow::GetInst()->showWindow();
}

void ChangeScene(DWORD_PTR, DWORD_PTR)
{
	ChangeScene(SCENE_TYPE::START);
}


void CScene_Tool::update()
{
	CScene::update();
	CToolWindow::GetInst()->update();

	SetTileIdx();

	//if (KEY_TAP(KEY::ENTER))
	//{
	//	ChangeScene(SCENE_TYPE::START);
	//}

	if (KEY_TAP(KEY::F1))
	{
		CToolWindow::GetInst()->showWindow();
	}

	if (KEY_TAP(KEY::F2))
	{
		ChangeScene(SCENE_TYPE::ANIMATION_WORKSHOP);
	}

	if (KEY_TAP(KEY::LBTN) && CKeyMgr::GetInst()->IsMouseInsideClinet(CToolWindow::GetInst()->GetToolhWnd()))
	{
		Vec2 vPos = MOUSE_POS;
	}

	//if (KEY_TAP(KEY::LSHIFT))
	//{
	//	//CUIMgr::GetInst()->SetFocusedUI(m_pUI);

	//	SaveTileData();
	//}

	if (KEY_TAP(KEY::CTRL))
	{
		//CUIMgr::GetInst()->SetFocusedUI(m_pUI);
		LoadTileData();
	}

}

void CScene_Tool::render(HDC _dc)
{
	CScene::render(_dc);
	CToolWindow::GetInst()->render();
	//DrawGizmo(_dc);
}


void CScene_Tool::SetTileIdx()
{
	if (KEY_TAP(KEY::LBTN))
	{
		Vec2 vMousePos = MOUSE_POS;
		vMousePos = CCamera::GetInst()->GetRealPos(vMousePos);
		
		UINT iTileX = (int)GetTileX();
		UINT iTileY = (int)GetTileY();

		UINT iCol = (int)vMousePos.x / TILE_SIZE;
		UINT iRow = (int)vMousePos.y / TILE_SIZE;
	
		if (vMousePos.x < 0.f || iTileX <= iCol
			|| vMousePos.y < 0.f || iTileY <= iRow) return;

		UINT iIdx = iRow * iTileX + iCol;

		const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);
		((CTile*)vecTile[iIdx])->AddImgIdx();
	}
}

void CScene_Tool::SaveTileData()
{
	OPENFILENAME ofn = {};
	
	wchar_t szName[256] = {};

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();
	ofn.lpstrFile = szName;
	ofn.nMaxFile = sizeof(szName);
	ofn.lpstrFilter = L"ALL\0*.*\0Tile\0*.tile\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
	strTileFolder += L"Tile";

	ofn.lpstrInitialDir = strTileFolder.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Modal 방식
	if (GetSaveFileName(&ofn))
	{
		SaveTile(szName);
	};
}

void CScene_Tool::LoadTileData()
{
	OPENFILENAME ofn = {};

	wchar_t szName[256] = {};

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();
	ofn.lpstrFile = szName;
	ofn.nMaxFile = sizeof(szName);
	ofn.lpstrFilter = L"ALL\0*.*\0Tile\0*.tile\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
	strTileFolder += L"Tile";

	ofn.lpstrInitialDir = strTileFolder.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Modal 방식
	if (GetOpenFileName(&ofn))
	{
		wstring strRelativePath = CPathMgr::GetInst()->GetRelativePath(szName);
		LoadTile(strRelativePath);
	};
}

void CScene_Tool::SaveTile(const wstring& _strFilePath)
{
	// 커널 오브젝트
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _strFilePath.c_str(), L"wb");

	assert(pFile);

	// 타일 가로 세로 개수 저장
	UINT xCount = GetTileX();
	UINT yCount = GetTileY();

	fwrite(&xCount, sizeof(UINT), 1, pFile);
	fwrite(&yCount, sizeof(UINT), 1, pFile);

	// 모든 타일들의 개별 데이터를 저장한다.
	const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

	for (size_t i = 0; i < vecTile.size(); i++)
	{
		((CTile*)vecTile[i])->Save(pFile);
	}

	fclose(pFile);
}

void CScene_Tool::DrawGizmo(HDC _dc)
{
	if (m_bGizmo)
	{
		Graphics graphics(_dc);
		Pen pen(Color(242, 242, 242));
		int gridSize = 10; // 각 셀의 크기
		for (int x = 0; x < 10000; x += gridSize) {
			Vec2 v1 = Vec2(x, 0);
			Vec2 v2 = Vec2(x, 10000);
			v1 = CCamera::GetInst()->GetRenderPos(v1);
			v2 = CCamera::GetInst()->GetRenderPos(v2);
			graphics.DrawLine(&pen, v1.x, v1.y, v2.x, v2.y); // 세로선 그리기
		}

		for (int y = 0; y < 10000; y += gridSize) {
			Vec2 y1 = Vec2(0, y);
			Vec2 y2 = Vec2(10000, y);
			y1 = CCamera::GetInst()->GetRenderPos(y1);
			y2 = CCamera::GetInst()->GetRenderPos(y2);
			graphics.DrawLine(&pen, y1.x, y1.y, y2.x, y2.y); // 가로선 그리기
		}
	}
}


void CScene_Tool::Exit()
{
	CCore::GetInst()->DivideMenu();

	DeleteAll();

	CCollisionMgr::GetInst()->Reset();

	CToolWindow::GetInst()->Destroy_window();
}


// ==============================================================================================
// Tile Count Window Proc
// ==============================================================================================

INT_PTR CALLBACK TileCountProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			// 프로시저에서 바로 처리
			UINT iXCount = GetDlgItemInt(hDlg, IDC_EDIT1, nullptr, false);
			UINT iYCount = GetDlgItemInt(hDlg, IDC_EDIT2, nullptr, false);

			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

			// dynamic cast는 형변환에 실패했을 경우에 nullptr이 나오게 된다.
			CScene_Tool* pToolScene = dynamic_cast<CScene_Tool*>(pCurScene);
			assert(pToolScene);

			pToolScene->DeleteGroup(GROUP_TYPE::TILE);
			pToolScene->CreateTile(iXCount, iYCount);

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
