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
#include "CBackground.h"
#include "CGround.h"


#include "CComponent.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CToolWindow.h"


void ChangeScene(DWORD_PTR, DWORD_PTR);
void G_ChangeTool(DWORD_PTR _i, DWORD_PTR);
void G_CreateMap(DWORD_PTR, DWORD_PTR);
void Save(DWORD_PTR, DWORD_PTR);
void Load(DWORD_PTR, DWORD_PTR);
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
	CCore::GetInst()->ChangeWindowSize(Vec2(1920, 1080), false);
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

	pBtnUI = new CBtnUI;
	pBtnUI->SetName(L"Load");
	pBtnUI->SetScale(Vec2(55.f, 25.f));
	pBtnUI->SetPos(Vec2(5.f, 95.f));
	//pBtnUI->SetClickCallBack(this, (SCENE_MEMFUNC)&CScene_Tool::toggleGizmo);
	pBtnUI->SetClickCallBack(Load, 0, 0);

	pPanelUI->AddChild(pBtnUI);

	pBtnUI = new CBtnUI;
	pBtnUI->SetName(L"Save");
	pBtnUI->SetScale(Vec2(55.f, 25.f));
	pBtnUI->SetPos(Vec2(5.f, 125.f));
	//pBtnUI->SetClickCallBack(this, (SCENE_MEMFUNC)&CScene_Tool::SaveSceneData);
	pBtnUI->SetClickCallBack(Save, 0, 0);

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

void Save(DWORD_PTR, DWORD_PTR)
{
	OPENFILENAME ofn = {};

	wchar_t szName[256] = {};

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();
	ofn.lpstrFile = szName;
	ofn.nMaxFile = sizeof(szName);
	ofn.lpstrFilter = L"scene\0*.scene\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	wstring strAnimationFolder = CPathMgr::GetInst()->GetContentPath();
	strAnimationFolder += L"Scene";

	ofn.lpstrInitialDir = strAnimationFolder.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// 확장자 지정
	ofn.lpstrDefExt = L"scene";

	// Modal 방식
	if (GetSaveFileName(&ofn))
	{
		CScene_Tool* pCurScene = (CScene_Tool*)CSceneMgr::GetInst()->GetCurScene();
		pCurScene->SaveSceneData(szName);
	}
}

void Load(DWORD_PTR, DWORD_PTR)
{
	OPENFILENAME ofn = {};

	wchar_t szName[256] = {};

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();
	ofn.lpstrFile = szName;
	ofn.nMaxFile = sizeof(szName);
	ofn.lpstrFilter = L"scene\0*.scene\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	// 컨텐츠 Path를 가져옴
	wstring strAnimationFolder = CPathMgr::GetInst()->GetContentPath();
	strAnimationFolder += L"Scene";

	ofn.lpstrInitialDir = strAnimationFolder.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// 파일 읽기가 성공했다면.
	if (GetOpenFileName(&ofn))
	{
		CScene_Tool* pCurScene = (CScene_Tool*)CSceneMgr::GetInst()->GetCurScene();
		pCurScene->LoadSceneData(szName);
	}
}

void CScene_Tool::update()
{
	CScene::update();
	CToolWindow::GetInst()->update();

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

void CScene_Tool::SaveSceneData(const wstring& _strFilePath)
{
	// 파일 이름 가져오기 (확장자 제외)
	wstring strName = L"";
	bool flag = 0;
	for (int i = _strFilePath.length() - 1; i >= 0; i--)
	{
		if (_strFilePath[i] == L'\\') break;
		if (flag) strName = _strFilePath[i] + strName;
		if (_strFilePath[i] == L'.') flag = 1;
	}

	CScene* pCurScene = this;

	FILE* pFile = nullptr;

	//// 파일스트림 열기
	_wfopen_s(&pFile, _strFilePath.c_str(), L"wb");
	assert(pFile);

	// 맵의 너비와 높이 저장
	UINT width = m_vMap.x;
	UINT Height = m_vMap.y;

	fwrite(&width, sizeof(UINT), 1, pFile);
	fwrite(&Height, sizeof(UINT), 1, pFile);

	const vector<CObject*> vecBG1 = GetGroupObject(GROUP_TYPE::BACKGROUND1);
	SaveObject(vecBG1, pFile);

	const vector<CObject*> vecBG2 = GetGroupObject(GROUP_TYPE::BACKGROUND2);
	SaveObject(vecBG2, pFile);

	const vector<CObject*> vecBG3 = GetGroupObject(GROUP_TYPE::BACKGROUND3);
	SaveObject(vecBG3, pFile);

	const vector<CObject*> vecFH = GetGroupObject(GROUP_TYPE::FOOTHOLD);
	SaveObject(vecFH, pFile);

	const vector<CObject*> vecGD = GetGroupObject(GROUP_TYPE::GROUND);
	SaveObject(vecGD, pFile);

	const vector<CObject*> vecMS = GetGroupObject(GROUP_TYPE::MONSTER);
	SaveObject(vecMS, pFile);

	fclose(pFile);
}

void CScene_Tool::LoadSceneData(const wstring& _strFilePath)
{
	// 로드하기 전에 맵을 초기화 한다.
	void ResetMap();
	DeleteAll_Except_UI();

	wstring fileName = L"";

	for (int i = _strFilePath.size() - 1; i > 0; i--)
	{
		if (_strFilePath[i] == L'\\') break;
		fileName = _strFilePath[i] + fileName;
	}

	CScene::LoadSceneData(fileName);

	m_vMap = GetMapSize();
}



void CScene_Tool::CreateMap(int _iWidth, int _iHeight)
{
	ResetMap();

	m_vMap = Vec2(_iWidth, _iHeight);
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
	CCollider* pCol = pObj->GetComponent()->GetCollider();
	pCol->SetScale(Vec2(rectWidth, rectHeight));
	AddObject(pObj, GROUP_TYPE::FOOTHOLD);
}

void CScene_Tool::DrawGizmo(HDC _dc)
{
	if (m_bGizmo)
	{
		Graphics graphics(_dc);
		Pen pen(Color(240, 240, 240), 1);

		int gridSize = 10; // 각 셀의 크기
		for (int x = 0; x <= m_vMap.x; x += gridSize) {
			Vec2 v1 = Vec2(x, 0);
			Vec2 v2 = Vec2((float)x, m_vMap.y);
			v1 = CCamera::GetInst()->GetRenderPos(v1);
			v2 = CCamera::GetInst()->GetRenderPos(v2);
			graphics.DrawLine(&pen, v1.x, v1.y, v2.x, v2.y); // 세로선 그리기
		}

		for (int y = 0; y <= m_vMap.y; y += gridSize) {
			Vec2 y1 = Vec2(0, y);
			Vec2 y2 = Vec2(m_vMap.x, (float)y);
			y1 = CCamera::GetInst()->GetRenderPos(y1);
			y2 = CCamera::GetInst()->GetRenderPos(y2);
			graphics.DrawLine(&pen, y1.x, y1.y, y2.x, y2.y); // 가로선 그리기
		}
	}
}

void CScene_Tool::renderMapBase(HDC _dc)
{
	Graphics graphics(_dc);



	if (m_vMap != Vec2(0.f, 0.f))
	{
		Vec2 vResolution = CCore::GetInst()->GetResolution();

		Vec2 vLT = -vResolution;
		Vec2 vRB = m_vMap + vResolution;

		Vec2 vScale = vRB - vLT;
		Vec2 vPos = -vScale / 2.f;

		// 출력할 랜더링 좌표로 변환
		vPos = CCamera::GetInst()->GetRenderPos(vPos);

		// map의 사이즈에 해상도만큼의 패딩처리를 하여 색칠한다.
		SolidBrush paddingBrush(Color(220, 220, 220, 220));
		graphics.FillRectangle(&paddingBrush, vPos.x, vPos.y, vScale.x, vScale.y);

		// 실제 맵이 그려질 공간
		vPos = (m_vMap / 2.f);
		vScale = m_vMap;

		vPos = CCamera::GetInst()->GetRenderPos(-(vScale / 2.f));
		
		GraphicsPath path;
		path.AddRectangle(Rect(vPos.x, vPos.y, vScale.x, vScale.y));

		SolidBrush fillBrush(Color(255, 255, 255));
		graphics.FillRegion(&fillBrush, new Region(&path));
	}

	// 중앙 표시
	Vec2 vCenter = Vec2(0.f, 0.f);
	vCenter = CCamera::GetInst()->GetRenderPos(vCenter);
	Pen pen(Color(0, 0, 0), 3.f);
	graphics.DrawRectangle(&pen, vCenter.x - 2.5f, vCenter.y - 2.5f, 5.f, 5.f);
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