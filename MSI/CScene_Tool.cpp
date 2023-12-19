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
#include "CCamera.h"

#include "CPathMgr.h"
#include "CFoothold.h"

#include "CToolWindow.h"


void ChangeScene(DWORD_PTR, DWORD_PTR);
void G_ChangeTool(DWORD_PTR _i, DWORD_PTR);
void G_CreateMap(DWORD_PTR, DWORD_PTR);
INT_PTR CALLBACK CreateMapDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

CScene_Tool::CScene_Tool()
	: m_bGizmo(false)
	, m_eType(MOUSE_TOOL_TYPE::DEFAULT)
	, m_bDrag(false)
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

	CUI* pPanelUI = new CPanelUI;
	pPanelUI->SetName(L"ParentUI");
	pPanelUI->SetScale(Vec2(65.f, 500.f));
	pPanelUI->SetPos(Vec2(vResolution.x - pPanelUI->GetScale().x, 0.f));

	CBtnUI* pBtnUI = new CBtnUI;
	pBtnUI->SetName(L"Create MAP");
	pBtnUI->SetScale(Vec2(55.f, 25.f));
	pBtnUI->SetPos(Vec2(5.f, 5.f));
	pBtnUI->SetClickCallBack(G_CreateMap, 0, 0);
	pPanelUI->AddChild(pBtnUI);

	pBtnUI = new CBtnUI;
	pBtnUI->SetName(L"▷");
	pBtnUI->SetScale(Vec2(25.f, 25.f));  
	pBtnUI->SetPos(Vec2(5.f, 35.f));
	//pBtnUI->SetClickCallBack(ChangeScene, 0, 0);
	pBtnUI->SetClickCallBack(G_ChangeTool, 0, 0);

	//pBtnUI->SetClickCallBack(this, (SCENE_MEMFUNC)&CScene_Tool::SaveTileData);

	pPanelUI->AddChild(pBtnUI);

	pBtnUI = new CBtnUI;
	pBtnUI->SetName(L"▶");
	pBtnUI->SetScale(Vec2(25.f, 25.f));
	pBtnUI->SetPos(Vec2(35.f, 35.f));
	pBtnUI->SetClickCallBack(G_ChangeTool, 1, 0);

	pPanelUI->AddChild(pBtnUI);

	pBtnUI = new CBtnUI;
	pBtnUI->SetName(L"Gizmo");
	pBtnUI->SetScale(Vec2(55.f, 25.f));
	pBtnUI->SetPos(Vec2(5.f, 65.f));
	pBtnUI->SetClickCallBack(this, (SCENE_MEMFUNC)&CScene_Tool::toggleGizmo);

	pPanelUI->AddChild(pBtnUI);







	AddObject(pPanelUI, GROUP_TYPE::UI);

	//CUI* pClonePanel = pPanelUI->Clone();
	//pClonePanel->SetPos(pClonePanel->GetPos() + Vec2(-300.f , 0.f));
	//AddObject(pClonePanel, GROUP_TYPE::UI);

	//m_pUI = pClonePanel;


	// 카메라 LookAt 지정
	CCamera::GetInst()->SetLookAt(vResolution / 2.f);

	CToolWindow::GetInst()->init();
	CToolWindow::GetInst()->showWindow();
}

void G_CreateMap(DWORD_PTR, DWORD_PTR)
{
	HINSTANCE hInst = CCore::GetInst()->GetMainhInstance();
	DialogBox(hInst, MAKEINTRESOURCE(IDD_CREATE_MAP), NULL, CreateMapDialogProc);
}

void ChangeScene(DWORD_PTR, DWORD_PTR)
{
	ChangeScene(SCENE_TYPE::START);
}

void G_ChangeTool(DWORD_PTR _i, DWORD_PTR)
{
	CScene_Tool* pCurScene = (CScene_Tool*) CSceneMgr::GetInst()->GetCurScene();
	pCurScene->ChangeTool((int)_i);
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

	FootHoldDrag();

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
	// Scene의 내부 요소들이 그려지기 전에 그려준다.
	renderMapBase(_dc);
	DrawGizmo(_dc);

	CScene::render(_dc);
	CToolWindow::GetInst()->render();

	Graphics graphics(_dc);



	Font font(L"Arial", 12);
	SolidBrush brush(Color(0, 0, 0));
	switch (m_eType)
	{
		case MOUSE_TOOL_TYPE::DEFAULT:
		{
			graphics.DrawString(L"Default", -1, &font, PointF(30.f, 100.f), &brush);
		}
	break;
		case MOUSE_TOOL_TYPE::FOOTHOLD:
		{
			graphics.DrawString(L"Create FootHold", -1, &font, PointF(30.f, 100.f), &brush);
		}
	break;
	}

	if(m_bGizmo) graphics.DrawString(L"Gizmo True", -1, &font, PointF(30.f, 150.f), &brush);
	else graphics.DrawString(L"Gizmo False", -1, &font, PointF(30.f, 150.f), &brush);

	renderDrag(_dc);
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

void CScene_Tool::CreateMap(int _iWidth, int _iHeight)
{
	ResetMap();

	m_vMapSize = Vec2(_iWidth, _iHeight);
}

void CScene_Tool::ResetMap()
{
	CSceneMgr::GetInst()->GetCurScene()->DeleteAll_Except_UI();
}

void CScene_Tool::FootHoldDrag()
{
	if (m_eType == MOUSE_TOOL_TYPE::FOOTHOLD && KEY_TAP(KEY::LBTN))
	{
		m_vDragStart = MOUSE_POS;
		m_bDrag = true;
	}

	if (m_eType == MOUSE_TOOL_TYPE::FOOTHOLD && KEY_AWAY(KEY::LBTN))
	{
		CreateFootHold();
		m_bDrag = false;
	}
}

void CScene_Tool::renderDrag(HDC _dc)
{
	if (m_bDrag && m_eType == MOUSE_TOOL_TYPE::FOOTHOLD)
	{
		Vec2 vDragEnd = MOUSE_POS;

		Graphics graphics(_dc);
		Pen pen(Color(0, 0, 0), 2.0f); // Red color, 2.0f thickness
		pen.SetDashStyle(DashStyleDash);  // Set the pen style to dashed

		float rectWidth = abs(vDragEnd.x - m_vDragStart.x);
		float rectHeight = abs(vDragEnd.y - m_vDragStart.y);
		graphics.DrawRectangle(&pen, min(m_vDragStart.x, vDragEnd.x), min(m_vDragStart.y, vDragEnd.y), rectWidth, rectHeight);
	}
}

void CScene_Tool::CreateFootHold()
{
	Vec2 vDragEnd = MOUSE_POS;

	Vec2 vCenter = (m_vDragStart + vDragEnd) / 2.f;
	vCenter = CCamera::GetInst()->GetRealPos(vCenter);

	float rectWidth = abs(vDragEnd.x - m_vDragStart.x);
	float rectHeight = abs(vDragEnd.y - m_vDragStart.y);

	if (rectWidth < 10.f || rectHeight < 10.f) return;

	CObject* pObj = new CFoothold;
	pObj->SetName(L"Foothold");
	pObj->SetPos(vCenter);
	pObj->SetScale(Vec2(rectWidth, rectHeight));
	AddObject(pObj, GROUP_TYPE::FOOTHOLD);
}

void CScene_Tool::DrawGizmo(HDC _dc)
{
	if (m_bGizmo)
	{
		Graphics graphics(_dc);
		Pen pen(Color(240, 240, 240), 1);

		int gridSize = 10; // 각 셀의 크기
		for (int x = 0; x <= m_vMapSize.x; x += gridSize) {
			Vec2 v1 = Vec2(x, 0);
			Vec2 v2 = Vec2((float)x, m_vMapSize.y);
			v1 = CCamera::GetInst()->GetRenderPos(v1);
			v2 = CCamera::GetInst()->GetRenderPos(v2);
			graphics.DrawLine(&pen, v1.x, v1.y, v2.x, v2.y); // 세로선 그리기
		}

		for (int y = 0; y <= m_vMapSize.y; y += gridSize) {
			Vec2 y1 = Vec2(0, y);
			Vec2 y2 = Vec2(m_vMapSize.x, (float)y);
			y1 = CCamera::GetInst()->GetRenderPos(y1);
			y2 = CCamera::GetInst()->GetRenderPos(y2);
			graphics.DrawLine(&pen, y1.x, y1.y, y2.x, y2.y); // 가로선 그리기
		}
	}
}

void CScene_Tool::renderMapBase(HDC _dc)
{
	if (m_vMapSize != Vec2(0.f, 0.f))
	{
		Vec2 vResolution = CCore::GetInst()->GetResolution();

		Vec2 vLT = -vResolution;
		Vec2 vRB = m_vMapSize + vResolution;

		Vec2 vScale = vRB - vLT;
		Vec2 vPos = -vScale / 2.f;

		// 출력할 랜더링 좌표로 변환
		vPos = CCamera::GetInst()->GetRenderPos(vPos);

		// map의 사이즈에 해상도만큼의 패딩처리를 하여 색칠한다.
		Graphics graphics(_dc);
		SolidBrush paddingBrush(Color(220, 220, 220, 220));
		graphics.FillRectangle(&paddingBrush, vPos.x, vPos.y, vScale.x, vScale.y);

		// 실제 맵이 그려질 공간
		vPos = (m_vMapSize / 2.f);
		vScale = m_vMapSize;

		vPos = CCamera::GetInst()->GetRenderPos(vPos);
		
		GraphicsPath path;
		path.AddRectangle(Rect(vPos.x - (vScale.x / 2.f), vPos.y - (vScale.y / 2.f), vScale.x, vScale.y));

		SolidBrush fillBrush(Color(255, 255, 255));
		graphics.FillRegion(&fillBrush, new Region(&path));
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


#include <string>

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


INT_PTR CALLBACK CreateMapDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
    case WM_INITDIALOG:
		{
			// 화면 크기 가져오기
			int screenWidth = GetSystemMetrics(SM_CXSCREEN);
			int screenHeight = GetSystemMetrics(SM_CYSCREEN);

			// 대화 상자 크기 가져오기
			RECT dlgRect;
			GetWindowRect(hwndDlg, &dlgRect);
			int dlgWidth = dlgRect.right - dlgRect.left;
			int dlgHeight = dlgRect.bottom - dlgRect.top;

			// 대화 상자를 화면 중앙에 위치시키기
			int x = (screenWidth - dlgWidth) / 2;
			int y = (screenHeight - dlgHeight) / 2;

			SetWindowPos(hwndDlg, NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
			return TRUE;	
		}
	break;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			TCHAR buffer1[256];
			TCHAR buffer2[256];
			GetWindowText(GetDlgItem(hwndDlg, IDC_EDIT_CREATE_MAP_WIDTH), buffer1, sizeof(buffer1) / sizeof(buffer1[0]));
			GetWindowText(GetDlgItem(hwndDlg, IDC_EDIT_CREATE_MAP_HEIGHT), buffer2, sizeof(buffer2) / sizeof(buffer2[0]));
			wstring w(buffer1);
			wstring h(buffer2);

			if (w == L"") w = L"100";
			if (h == L"") h = L"100";

			int width = stoi(w);
			int height = stoi(h);

			if (width < 500 || height < 500)
			{
				MessageBox(NULL, L"너비와 높이의 최소 크기는 500px 입니다.", L"알림", MB_OK | MB_ICONINFORMATION);
				if (width < 500) width = 500;
				if (height < 500) height = 500;
			}

			CScene_Tool* pCurScene = (CScene_Tool*)CSceneMgr::GetInst()->GetCurScene();
			pCurScene->CreateMap(width, height);

			EndDialog(hwndDlg, LOWORD(wParam));
			return TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hwndDlg, LOWORD(wParam));
			return TRUE;
		}

		break;
	}

	return FALSE;
}