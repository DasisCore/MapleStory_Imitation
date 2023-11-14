#include "global.h"

#include "CCore.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CSceneMgr.h"
#include "CPathMgr.h"
#include "CCollisionMgr.h"
#include "CEventMgr.h"
#include "CCamera.h"
#include "CUIMgr.h"
#include "CResMgr.h"
#include "CCamera.h"
#include "CSoundMgr.h"

#include "CSound.h"

#include "CTexture.h"

#include "SelectGDI.h"
#include "resource.h"

CCore::CCore()
	: m_hWnd{0}
	, m_ptResolution{0}
	, m_hDC{0}
	, m_arrBrush{}
	, m_arrPen{}
	, m_gdiplusToken(0)
{

}
CCore::~CCore()
{
	ReleaseDC(m_hWnd, m_hDC);

	for (int i = 0; i < (UINT)PEN_TYPE::END; i++)
	{
		DeleteObject(m_arrPen[i]);
	}
	
	DestroyMenu(m_hMenu);

	// 프로그램 종료시 GDI+ 종료
	GdiplusShutdown(m_gdiplusToken);
}

int CCore::init(HWND _hWnd, POINT _ptResolution, HINSTANCE _hInstance)
{
	// 기본 멤버 변수 초기화
	m_hWnd = _hWnd;
	m_ptResolution = _ptResolution;
	m_hDC = GetDC(m_hWnd);
	m_hInstance = _hInstance;

	// 해상도에 맞게 윈도우 크기 설정
	ChangeWindowSize(m_ptResolution, false);
	m_hMenu = LoadMenu(nullptr, MAKEINTRESOURCEW(IDC_MSI));


	// 맨 처음 윈도우의 위치를 화면 정 중앙으로 위치 시킨다.
	// 모니터의 해상도 얻기
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// 화면 정 중앙의 값 계산
	int centerX = (screenWidth - 1280) / 2;
	int centerY = (screenHeight - 720) / 2;

	RECT rt = { 0, 0, (long)_ptResolution.x, (long)_ptResolution.y };
	SetWindowPos(m_hWnd, nullptr, centerX, centerY, rt.right - rt.left, rt.bottom - rt.top, 0);


	// 메뉴바 삭제
	//SetMenu(m_hWnd, nullptr);

	// =======================================
	// 더블 버퍼링 용도의 텍스처 생성
	// =======================================
	m_pMemTex = CResMgr::GetInst()->CreateTexture(L"BackBuffer", (UINT)m_ptResolution.x, (UINT)m_ptResolution.y);


	// 자주 사용할 펜 및 브러쉬 초기화
	CreateBrushPen();


	// =======================================
	// 매니저 초기화
	// =======================================
	CPathMgr::GetInst()->init();
	CTimeMgr::GetInst()->init();
	CKeyMgr::GetInst()->init();
	CCamera::GetInst()->init();
	CSoundMgr::GetInst()->init();
	CSceneMgr::GetInst()->init();


	// 사운드 테스트 이후 fmod로 변경
	CResMgr::GetInst()->LoadSound(L"BGM_01", L"Sound\\DM.wav");
	//CSound* pNewSound = CResMgr::GetInst()->FindSound(L"BGM_01");

	//pNewSound->PlayToBGM(true);
	//pNewSound->Play();

	//pNewSound->SetPosition(0.f);	// 백분률, 소리 위치 설정 (재생 위치)
	//pNewSound->SetVolume(1.f);


	// PNG 이미지 처리를 위한 GDI+ 초기화
	GdiplusStartupInput gdiplusStartupInput;
	gdiplusStartupInput.SuppressBackgroundThread = FALSE;	// PNG의 ALPHA 처리
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	return S_OK;
}

void CCore::progress()
{
	// =======================================
	// 매 프레임 각 매니저 업데이트
	// =======================================
	CTimeMgr::GetInst()->update();
	CKeyMgr::GetInst()->update();
	CCamera::GetInst()->update();

	// Scene update
	CSceneMgr::GetInst()->update();

	// 충돌 체크
	CCollisionMgr::GetInst()->update();
	
	// UI 이벤트 체크
	CUIMgr::GetInst()->update();


	// =======================================
	// 렌더링 작업
	// =======================================

	HDC currentDC = m_pMemTex->GetDC();
	Clear();

	CSceneMgr::GetInst()->render(currentDC);
	CCamera::GetInst()->render(currentDC);

	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y, currentDC, 0, 0, SRCCOPY);


	// =======================================
	// 이벤트 지연처리
	// =======================================
	CEventMgr::GetInst()->update();
}

void CCore::CreateBrushPen()
{
	m_arrBrush[(UINT)BRUSH_TYPE::HOLLOW] = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	m_arrBrush[(UINT)BRUSH_TYPE::BLACK] = (HBRUSH)GetStockObject(BLACK_BRUSH);

	m_arrPen[(UINT)PEN_TYPE::RED] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_arrPen[(UINT)PEN_TYPE::BLUE] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	m_arrPen[(UINT)PEN_TYPE::GREEN] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
}

void CCore::Clear()
{
	HDC currentDC = m_pMemTex->GetDC();
	SelectGDI a(currentDC, BRUSH_TYPE::BLACK);
	Rectangle(currentDC, -1, -1, m_ptResolution.x + 11, m_ptResolution.y + 1);
}

void CCore::DockMenu()
{
	SetMenu(m_hWnd, m_hMenu);
	ChangeWindowSize(m_ptResolution, true);
}

void CCore::DivideMenu()
{
	SetMenu(m_hWnd, nullptr);
	ChangeWindowSize(m_ptResolution, false);
}

void CCore::ChangeWindowSize(Vec2 _vResolution, bool _bMenu)
{
	// 사각형을 나타내는 구조체 RECT
	RECT rt = { 0, 0, (long)_vResolution.x, (long)_vResolution.y };
	
	// 메뉴바 유무에 맞추어 window의 크기를 조정한다.
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, _bMenu);
	
	// 현재 윈도우의 위치를 얻어온다. 그 위치를 유지하면서 window의 사이즈를 조절한다.
	HWND MainhWnd = CCore::GetInst()->GetMainHwnd();
	RECT mainWndRect;
	GetWindowRect(MainhWnd, &mainWndRect);

	int mainWndX = mainWndRect.left;
	int mainWndY = mainWndRect.top;

	// 윈도우의 위치와 크기를 조정
	SetWindowPos(m_hWnd, nullptr, mainWndX, mainWndY, rt.right - rt.left, rt.bottom - rt.top, 0);
}

void CCore::render()
{
}
