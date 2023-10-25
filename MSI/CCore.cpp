#include "global.h"
#include "CCore.h"
#include "CKeyMgr.h"

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

	return S_OK;
}

void CCore::progress()
{
	Rectangle(m_memDC, -1, -1, m_ptResolution.x + 10, m_ptResolution.y + 10);

	// 임시로 누르는 키 화면에 나오게 함.
	KEY temp = CKeyMgr::GetInst()->GetInput();
	int a = (int)temp;
	string t = std::to_string(a);

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring myWString = converter.from_bytes(t);

	const wchar_t* myWchar = myWString.c_str();

	RECT rt = { 10, 10, 200, 100 };
	DrawText(m_memDC, myWchar, -1, &rt, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	// BitBlt 이용 m_hDC와 m_memDC를 서로 바꾸어준다.
	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y, m_memDC, 0, 0, SRCCOPY);

	//////////////////////////////////////////////
	// 매 프레임 각 매니저 업데이트
	//////////////////////////////////////////////
	CKeyMgr::GetInst()->update();
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
