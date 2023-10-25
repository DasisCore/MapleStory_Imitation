#include "global.h"

#include "CCore.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"

CCore::CCore()
	: m_hWnd{0}
	, m_ptResolution{0}
	, m_hDC{0}
	, m_hBit{0}
	, m_memDC{0}

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
	m_hDC = GetDC(m_hWnd);

	// 해상도에 맞게 윈도우 크기 설정
	ChangeWindowSize(GetResolution(), false);

	// 메뉴바 삭제
	SetMenu(m_hWnd, nullptr);

	///////////////////////////////////////////////
	// 더블 버퍼링을 위한 멤버 변수 초기화
	///////////////////////////////////////////////
	m_hBit = CreateCompatibleBitmap(m_hDC, m_ptResolution.x, m_ptResolution.y);
	m_memDC = CreateCompatibleDC(m_hDC);

	HBITMAP hOldBit = (HBITMAP)SelectObject(m_memDC, m_hBit);
	DeleteObject(hOldBit);

	///////////////////////////////////////////////
	// 매니저 초기화
	///////////////////////////////////////////////
	CKeyMgr::GetInst()->init();
	CTimeMgr::GetInst()->init();

	return S_OK;
}

void CCore::progress()
{

	//////////////////////////////////////////////
	// 매 프레임 각 매니저 업데이트
	//////////////////////////////////////////////
	CKeyMgr::GetInst()->update();
	CTimeMgr::GetInst()->update();



	render();

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
	Rectangle(m_memDC, -1, -1, m_ptResolution.x + 11, m_ptResolution.y + 1);

	static float x = 100.f;
	static float y = 100.f;

	if (KEY_HOLD(KEY::UP))
	{
		x -= 100.f * DT;
	}

	if (KEY_HOLD(KEY::RIGHT))
	{
		y += 100.f * DT;
	}

	if(KEY_HOLD(KEY::DOWN))
	{
		x += 100.f * DT;
	}

	if (KEY_HOLD(KEY::LEFT))
	{
		y -= 100.f * DT;
	}

	Rectangle(m_memDC, y, x, y + 100, x + 100);

	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y, m_memDC, 0, 0, SRCCOPY);
}
