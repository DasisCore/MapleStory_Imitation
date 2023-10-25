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
	// �⺻ ��� ���� �ʱ�ȭ
	m_hWnd = _hWnd;
	m_ptResolution = _ptResolution;
	m_hDC = GetDC(m_hWnd);

	// �ػ󵵿� �°� ������ ũ�� ����
	ChangeWindowSize(GetResolution(), false);

	// �޴��� ����
	SetMenu(m_hWnd, nullptr);


	///////////////////////////////////////////////
	// �Ŵ��� �ʱ�ȭ
	///////////////////////////////////////////////
	CKeyMgr::GetInst()->init();

	return S_OK;
}

void CCore::progress()
{
	Rectangle(m_hDC, -1, -1, m_ptResolution.x + 10, m_ptResolution.y + 10);

	//////////////////////////////////////////////
	// �� ������ �� �Ŵ��� ������Ʈ
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
	// �簢���� ��Ÿ���� ����ü
	RECT rt = { 0, 0, (long)_vResolution.x, (long)_vResolution.y };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, _bMenu);
	SetWindowPos(m_hWnd, nullptr, 100, 100, rt.right - rt.left, rt.bottom - rt.top, 0);
}
