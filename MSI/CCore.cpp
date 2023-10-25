#include "global.h"
#include "CCore.h"

CCore::CCore()
	: m_hWnd{0}
	, m_ptResolution{0}
	, m_hDC{0}
{

}
CCore::~CCore()
{

}

int CCore::init(HWND _hWnd, POINT _ptResolution)
{
	// 기본 멤버 변수 초기화
	m_hWnd = _hWnd;
	m_ptResolution = _ptResolution;

	// 해상도에 맞게 윈도우 크기 설정
	ChangeWindowSize(GetResolution(), false);

	// 메뉴바 삭제
	SetMenu(m_hWnd, nullptr);

	return S_OK;
}

void CCore::progress()
{

}

void CCore::ChangeWindowSize(Vec2 _vResolution, bool _bMenu)
{

	// 사각형을 나타내는 구조체
	RECT rt = { 0, 0, (long)_vResolution.x, (long)_vResolution.y };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, _bMenu);
	SetWindowPos(m_hWnd, nullptr, 100, 100, rt.right - rt.left, rt.bottom - rt.top, 0);
}
