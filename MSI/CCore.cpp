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

#include "CTexture.h"

#include "SelectGDI.h"
#include "resource.h"

CCore::CCore()
	: m_hWnd{0}
	, m_ptResolution{0}
	, m_hDC{0}
	, m_arrBrush{}
	, m_arrPen{}
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
}

int CCore::init(HWND _hWnd, POINT _ptResolution)
{
	// 기본 멤버 변수 초기화
	m_hWnd = _hWnd;
	m_ptResolution = _ptResolution;
	m_hDC = GetDC(m_hWnd);

	// 해상도에 맞게 윈도우 크기 설정
	ChangeWindowSize(m_ptResolution, false);
	m_hMenu = LoadMenu(nullptr, MAKEINTRESOURCEW(IDC_MSI));

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
	CSceneMgr::GetInst()->init();

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
	
	// 윈도우의 위치와 크기를 조정
	SetWindowPos(m_hWnd, nullptr, 100, 100, rt.right - rt.left, rt.bottom - rt.top, 0);
}

void CCore::render()
{
}
