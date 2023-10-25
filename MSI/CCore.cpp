#include "global.h"
#include "CCore.h"
#include "CKeyMgr.h"

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
	m_hDC = GetDC(m_hWnd);

	// 해상도에 맞게 윈도우 크기 설정
	ChangeWindowSize(GetResolution(), false);

	// 메뉴바 삭제
	SetMenu(m_hWnd, nullptr);


	///////////////////////////////////////////////
	// 매니저 초기화
	///////////////////////////////////////////////
	CKeyMgr::GetInst()->init();

	return S_OK;
}

void CCore::progress()
{
	Rectangle(m_hDC, -1, -1, m_ptResolution.x + 10, m_ptResolution.y + 10);

	//////////////////////////////////////////////
	// 매 프레임 각 매니저 업데이트
	//////////////////////////////////////////////
	CKeyMgr::GetInst()->update();




	KEY temp = CKeyMgr::GetInst()->GetInput();
	int a = (int)temp;
	string t = std::to_string(a);

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring myWString = converter.from_bytes(t);

	const wchar_t* myWchar = myWString.c_str();

	RECT rt = { 10, 10, 200, 100 };
	DrawText(m_hDC, myWchar, -1, &rt, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	//DrawText(m_hDC, TEXT("Hello, world"), -1, &rt, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}

void CCore::ChangeWindowSize(Vec2 _vResolution, bool _bMenu)
{
	// 사각형을 나타내는 구조체
	RECT rt = { 0, 0, (long)_vResolution.x, (long)_vResolution.y };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, _bMenu);
	SetWindowPos(m_hWnd, nullptr, 100, 100, rt.right - rt.left, rt.bottom - rt.top, 0);
}
