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
	// �⺻ ��� ���� �ʱ�ȭ
	m_hWnd = _hWnd;
	m_ptResolution = _ptResolution;
	m_hDC = GetDC(m_hWnd);

	// �ػ󵵿� �°� ������ ũ�� ����
	ChangeWindowSize(GetResolution(), false);

	// �޴��� ����
	SetMenu(m_hWnd, nullptr);

	///////////////////////////////////////////////
	// ���� ���۸��� ���� ��� ���� �ʱ�ȭ
	///////////////////////////////////////////////
	m_hBit = CreateCompatibleBitmap(m_hDC, m_ptResolution.x, m_ptResolution.y);
	m_memDC = CreateCompatibleDC(m_hDC);

	HBITMAP hOldBit = (HBITMAP)SelectObject(m_memDC, m_hBit);
	DeleteObject(hOldBit);



	///////////////////////////////////////////////
	// �Ŵ��� �ʱ�ȭ
	///////////////////////////////////////////////
	CKeyMgr::GetInst()->init();

	return S_OK;
}

void CCore::progress()
{
	Rectangle(m_memDC, -1, -1, m_ptResolution.x + 10, m_ptResolution.y + 10);

	// �ӽ÷� ������ Ű ȭ�鿡 ������ ��.
	KEY temp = CKeyMgr::GetInst()->GetInput();
	int a = (int)temp;
	string t = std::to_string(a);

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring myWString = converter.from_bytes(t);

	const wchar_t* myWchar = myWString.c_str();

	RECT rt = { 10, 10, 200, 100 };
	DrawText(m_memDC, myWchar, -1, &rt, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	// BitBlt �̿� m_hDC�� m_memDC�� ���� �ٲپ��ش�.
	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y, m_memDC, 0, 0, SRCCOPY);

	//////////////////////////////////////////////
	// �� ������ �� �Ŵ��� ������Ʈ
	//////////////////////////////////////////////
	CKeyMgr::GetInst()->update();
}

void CCore::ChangeWindowSize(Vec2 _vResolution, bool _bMenu)
{
	// �簢���� ��Ÿ���� ����ü RECT
	RECT rt = { 0, 0, (long)_vResolution.x, (long)_vResolution.y };
	
	// �޴��� ������ ���߾� window�� ũ�⸦ �����Ѵ�.
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, _bMenu);
	
	// �������� ��ġ�� ũ�⸦ ����
	SetWindowPos(m_hWnd, nullptr, 100, 100, rt.right - rt.left, rt.bottom - rt.top, 0);
}
